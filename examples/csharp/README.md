# Using DX-RT from C#

DX-RT does **not** ship an official C# package. C# applications call the
stable C API (`dxrt_*` functions exported by `dxrt.dll`) directly with
P/Invoke.

This directory contains guidance only; it has no project to build. The former
`csharp_package` (C++/CLI bridge + NuGet project) was removed because it
depended on the C++ ABI instead of the stable C API.

## Source of truth

- Header: [`lib/include/dxrt/dxrt_c_api.h`](../../lib/include/dxrt/dxrt_c_api.h)
  (installed as `include/dxrt/dxrt_c_api.h`).
- Function signatures, struct definitions, ownership, and lifetime rules come
  from that header. Do not copy declarations from older C# code.
- Do not bind declarations marked `@internal` in the header (for example
  `dxrt_engine_register_release_callback`). They are not part of the public
  API.

## Runtime files

- Target **x64** only (`<PlatformTarget>x64</PlatformTarget>`).
- Place the DLLs from the install `bin` directory next to your executable, or
  on `PATH`. For a source build (`build.bat install`) this is
  `out\install\x64-Release\bin\`, which contains `dxrt.dll`,
  `onnxruntime.dll`, and `onnxruntime_providers_shared.dll`.
- The Windows build uses the `dxrtd` service. See
  [Installation on Windows](../../docs/source/docs/03_Installation_on_Windows.md).

## Binding rules

### Calling convention and errors

- All functions use the C calling convention: use
  `CallingConvention.Cdecl` and the library name `"dxrt"`.
- Most functions return `dxrt_status_t` (`int`; `0` = `DXRT_OK`). On failure,
  call `dxrt_last_error_message()` **on the same thread, before any other
  `dxrt_*` call**, and copy the string. The message is thread-local and the
  pointer is invalidated by the next `dxrt_*` call. Do not use
  `SetLastError`/`Marshal.GetLastWin32Error`.

### `static inline` helpers are not exported

`dxrt_options_init()`, `dxrt_tensor_slice_init()`, and
`dxrt_npu_timing_init()` are `static inline` in the header. They do not exist
in `dxrt.dll`, so a P/Invoke to them fails with `EntryPointNotFoundException`.
Re-implement them in C#: zero the struct, set `struct_size`, then apply the
defaults written in the header.

### Struct layout

- Use `[StructLayout(LayoutKind.Sequential)]` with default packing. Do **not**
  set `Pack = 1`; the C layout contains alignment padding.
- Set `struct_size` from the C# size (`Marshal.SizeOf<T>()`) and check it
  against the values below.

Sizes measured with MSVC x64 against `dxrt_c_api.h`:

| C struct | Size | Notes |
|---|---|---|
| `dxrt_options_t` | 44 | Size is fixed by `static_assert`. |
| `dxrt_tensor_info_t` | 344 | `name[256]` at 0, `type` at 256, 4 bytes padding, `shape[8]` at 272, `size_in_bytes` at 336. |
| `dxrt_tensor_slice_t` | 384 | `data` at 8 (4 bytes padding after `struct_size`), `info` at 24, `reserved` at 368. |
| `dxrt_npu_timing_t` | 88 | Size is negotiated through `struct_size`. |

### Strings

- Input paths are UTF-8: marshal them with
  `[MarshalAs(UnmanagedType.LPUTF8Str)]`.
- Strings returned as `const char*` are owned by the library: receive them as
  `IntPtr`, copy them with `Marshal.PtrToStringUTF8`, and never free them.
- Functions that fill a caller buffer (for example
  `dxrt_engine_get_model_name`) take a `byte[]` and its length.

### Handles and buffers

- Wrap `dxrt_engine_t` in a `SafeHandle` whose `ReleaseHandle` calls
  `dxrt_engine_destroy`.
- Query buffer sizes with `dxrt_engine_get_input_size` and
  `dxrt_engine_get_output_size`.
- For synchronous calls, blittable arrays such as `byte[]` are pinned for the
  duration of the call.
- For `dxrt_engine_run_async`, input and output buffers must stay pinned
  (`GCHandle.Alloc(..., GCHandleType.Pinned)` or native memory) until the job
  completes through `dxrt_engine_wait` or the callback.

### Callbacks

These apply to `dxrt_engine_register_callback` and
`dxrt_event_register_handler`:

- **Keep the delegate referenced.** The header has no unregister function, so
  keep it alive for as long as the runtime can call it (at least the engine
  lifetime for engine callbacks, and the process lifetime for event handlers).
  A collected delegate crashes the process when the runtime calls it.
- Declare the delegate with
  `[UnmanagedFunctionPointer(CallingConvention.Cdecl)]`.
- `dxrt_tensor_slice_t.data` is valid only during the callback. Copy the data
  before returning.
- Never let an exception escape a callback. Catch everything inside it.
- The header does not document which thread invokes callbacks, whether
  callbacks can run concurrently, or the meaning of the `int` returned by
  `dxrt_callback_fn`. Assume a runtime worker thread with possible concurrency,
  and do not touch UI objects directly from the callback.

## Minimal example

```csharp
using System;
using System.Runtime.InteropServices;

internal sealed class DxrtEngineHandle : SafeHandle
{
    public DxrtEngineHandle() : base(IntPtr.Zero, ownsHandle: true) { }
    public override bool IsInvalid => handle == IntPtr.Zero;
    protected override bool ReleaseHandle()
    {
        Dxrt.dxrt_engine_destroy(handle);
        return true;
    }
}

internal static class Dxrt
{
    private const string Lib = "dxrt";

    [StructLayout(LayoutKind.Sequential)]
    public struct Options
    {
        public uint StructSize;
        public int BufferCount;
        public int DeviceId;
        public uint BoundOption;
        public int UseOrt;
        public int ShowModelInfo;
        public uint Reserved0, Reserved1, Reserved2, Reserved3, Reserved4;

        // Must match sizeof(dxrt_options_t), which the header pins with static_assert.
        public const int NativeSize = 44;

        // Mirrors the header-only dxrt_options_init().
        public static Options CreateDefault()
        {
            int size = Marshal.SizeOf<Options>();
            if (size != NativeSize)
                throw new InvalidOperationException(
                    $"Options layout mismatch: {size} bytes, expected {NativeSize}");

            return new Options
            {
                StructSize = (uint)size,
                BufferCount = 0,
                DeviceId = -1,
                BoundOption = 0,
                UseOrt = -1,
                ShowModelInfo = -1,
            };
        }
    }

    [DllImport(Lib, CallingConvention = CallingConvention.Cdecl)]
    private static extern IntPtr dxrt_last_error_message();

    [DllImport(Lib, CallingConvention = CallingConvention.Cdecl)]
    public static extern int dxrt_engine_create(
        [MarshalAs(UnmanagedType.LPUTF8Str)] string modelPath,
        in Options options,
        out DxrtEngineHandle engine);

    [DllImport(Lib, CallingConvention = CallingConvention.Cdecl)]
    public static extern void dxrt_engine_destroy(IntPtr engine);

    [DllImport(Lib, CallingConvention = CallingConvention.Cdecl)]
    public static extern int dxrt_engine_get_input_size(DxrtEngineHandle engine, out ulong size);

    [DllImport(Lib, CallingConvention = CallingConvention.Cdecl)]
    public static extern int dxrt_engine_get_output_size(DxrtEngineHandle engine, out ulong size);

    [DllImport(Lib, CallingConvention = CallingConvention.Cdecl)]
    public static extern int dxrt_engine_run(DxrtEngineHandle engine, byte[] input, [Out] byte[] output);

    public static void Check(int status)
    {
        if (status != 0)
        {
            string message = Marshal.PtrToStringUTF8(dxrt_last_error_message()) ?? string.Empty;
            throw new InvalidOperationException($"DX-RT error {status}: {message}");
        }
    }
}

internal static class Program
{
    private static void Main(string[] args)
    {
        var options = Dxrt.Options.CreateDefault();
        Dxrt.Check(Dxrt.dxrt_engine_create(args[0], options, out var engine));
        using (engine)
        {
            Dxrt.Check(Dxrt.dxrt_engine_get_input_size(engine, out ulong inputSize));
            Dxrt.Check(Dxrt.dxrt_engine_get_output_size(engine, out ulong outputSize));

            var input = new byte[inputSize];
            var output = new byte[outputSize];
            Dxrt.Check(Dxrt.dxrt_engine_run(engine, input, output));

            Console.WriteLine($"input={inputSize} bytes, output={outputSize} bytes");
        }
    }
}
```

## Generating a full binding with an AI agent

The header is designed to be machine-readable. A prompt such as the following
works as a starting point:

> Using `lib/include/dxrt/dxrt_c_api.h` as the only source of truth, write C#
> P/Invoke bindings for DX-RT. Follow `examples/csharp/README.md`: Cdecl,
> skip `@internal` declarations, re-implement the `static inline` init
> helpers, sequential layout with
> default packing, verify struct sizes against the README table, UTF-8
> strings, `SafeHandle` for `dxrt_engine_t`, and keep callback delegates
> alive.

Review the generated struct layouts against the table above, and test the
binding with your model before using it in production.
