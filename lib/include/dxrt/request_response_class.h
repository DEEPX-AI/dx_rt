
#pragma once

#include "dxrt/common.h"

// Project headers
#include "dxrt/request.h"
#include "dxrt/task.h"


namespace dxrt {

class DXRT_API RequestResponse {
 public:
    static int InferenceRequest(RequestPtr req);
    static void ProcessByData(int reqId, const dxrt_response_t& response, int deviceId);
    static void ProcessByDataNormal(RequestPtr req, const dxrt_response_t& response, int deviceId);
    static void ProcessByDataArgmax(RequestPtr req, const dxrt_response_t& response, int deviceId);
    static void ProcessByDataPPU(RequestPtr req, const dxrt_response_t& response, int deviceId);
    static void ProcessByDataPPCPU(RequestPtr req, const dxrt_response_t& response, int deviceId);
    static int ProcessResponse(RequestPtr req, const dxrt_response_t& response, int deviceType);
#ifdef DXRT_USE_DEVICE_VALIDATION
    static int ValidateRequest(RequestPtr req);
#endif
};

}  // namespace dxrt
