// RUN: ttmlir-opt --ttir-to-ttnn-backend-pipeline %s | FileCheck %s

module attributes {} {
  func.func @logical_xor(%arg0: tensor<64x128xbf16>, %arg1: tensor<64x128xbf16>) -> tensor<64x128xbf16> {
    // CHECK: %{{[0-9]+}} = "ttnn.empty"{{.*}} [[TENSOR:tensor<64x128xbf16]]
    %0 = tensor.empty() : tensor<64x128xbf16>
    // CHECK: %{{[0-9]+}} = "ttnn.logical_xor"
    // CHECK-SAME: [[TENSOR]]
    // CHECK-SAME: [[TENSOR]]
    // CHECK-SAME: [[TENSOR]]
    // CHECK-SAME: -> [[TENSOR]]
    %1 = "ttir.logical_xor"(%arg0, %arg1, %0) <{operandSegmentSizes = array<i32: 2, 1>}> : (tensor<64x128xbf16>, tensor<64x128xbf16>, tensor<64x128xbf16>) -> tensor<64x128xbf16>
    return %1 : tensor<64x128xbf16>
  }
}
