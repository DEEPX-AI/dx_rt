#!/bin/bash

function help()
{
    echo "$0 [ options ]"
    echo "    --help     show this help"
    echo "    --out      release dir."
}

function build()
{
    cmd=()
    clean_build=$1
    verbose=$2
    build_type=$3
    target_arch=$4
    dev_build=$5
    [ $clean_build == "true" ] && cmd+=(--clean )
    [ $verbose == "true" ] && cmd+=(--verbose )
    cmd+=(--type $build_type )
    cmd+=(--arch $target_arch )
    [ $dev_build == "true" ] && cmd+=(--dev )
    bash build.sh ${cmd[@]} && {
        local id="$target_arch"
        build_dir=build_"$id"
        cp -r $build_dir/release/include/dxrt $out_dir/lib/include
        cp -r $build_dir/release/lib/libdxrt_$id.a $out_dir/lib/lib/
        cp -r $build_dir/release/lib/libdxrt_$id.so $out_dir/lib/lib/
    }
}

dxrt_ver=`cat release.ver`
release_info=release.info
out_dir=dx_rt
out_dir_archive_file=dx_rt_$dxrt_ver.tar.gz
[ $# -gt 0 ] && \
while (( $# )); do
    case "$1" in
        --help)  help; exit 0;;
        --out)
            shift
            out_dir=$1
            shift;;
        *)       echo "Invalid argument : " $1 ; help; exit 1;;
    esac
done

rm -rf $out_dir
mkdir $out_dir
cp ChangeLog.md $out_dir/
cp -r app $out_dir/
cp -r cmake $out_dir/
cp -r docs $out_dir/
cp -r driver $out_dir/
cp -r extern $out_dir/
cp -r lib $out_dir/
cp -r sample $out_dir/
cp -r test $out_dir/
mkdir -p $out_dir/tool
cp -r tool/profiler $out_dir/tool/
cp -r tool/ssd_priorbox $out_dir/tool/
cp -r build.sh $out_dir/
cp -r build.cfg $out_dir/

cp CMakeLists.txt $out_dir/
cp build.sh $out_dir/
cp mkdocs.yml $out_dir/

echo "  dxrt version : $dxrt_ver" > $release_info
echo "  release file : $out_dir_archive_file" >> $release_info
echo "  release date : $(date)" >> $release_info
echo "  git hash : $(git rev-parse --short HEAD)" >> $release_info

cp $release_info $out_dir
cp release.ver $out_dir
cp install.sh $out_dir
tar zcvf $out_dir_archive_file $out_dir

echo =================================================
cat $out_dir/$release_info
echo =================================================
