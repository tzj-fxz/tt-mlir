// SPDX-FileCopyrightText: (c) 2024 Tenstorrent AI ULC
//
// SPDX-License-Identifier: Apache-2.0

#include "to_layout.h"
#include "tt/runtime/detail/logger.h"
#include "tt/runtime/detail/ttnn.h"
#include "tt/runtime/ttnn/operations/utils.h"
#include "tt/runtime/ttnn/utils.h"

namespace tt::runtime::ttnn::operations::layout {

static ::ttnn::Layout toTTNNLayout(::tt::target::TensorLayout layout) {
  switch (layout) {
  case ::tt::target::TensorLayout::RowMajor:
    return ::ttnn::Layout::ROW_MAJOR;
  case ::tt::target::TensorLayout::Tile:
    return ::ttnn::Layout::TILE;
  case ::tt::target::TensorLayout::Invalid:
    return ::ttnn::Layout::INVALID;
  }
}

void run(const ::tt::target::ttnn::ToLayoutOp *op, ProgramContext &context) {
  ProgramTensorPool &tensorPool = context.getTensorPool();
  const ::ttnn::Tensor &inputTensor = tensorPool.at(op->in()->global_id());
  DEBUG_ASSERT(inputTensor.is_allocated());
  const ::tt::target::Dim2d *targetTileShape =
      op->out()->desc()->layout()->memory_desc()->tile_shape();
  LOG_ASSERT(::tt::runtime::ttnn::utils::isValidTileShape(targetTileShape),
             "Invalid tile shape");

  ::ttnn::Layout layout = toTTNNLayout(op->layout());
  std::optional<::ttnn::DataType> dtype = std::nullopt;
  std::optional<::ttnn::MemoryConfig> memoryConfig = std::nullopt;
  ::ttnn::Device *device = nullptr;

  if (op->dtype()) {
    dtype = ::tt::runtime::ttnn::utils::toTTNNDataType(*(op->dtype()));
  }

  if (op->memcfg()) {
    memoryConfig =
        std::make_optional(utils::createMemoryConfig(op->memcfg(), op->out()));
  }

  if (op->device()) {
    device = &context.getDeviceFromView(op->device()->global_id(), 0);
  }

  ::ttnn::Tensor out =
      ::ttnn::to_layout(inputTensor, layout, dtype, memoryConfig, device);

  if (tensorPool.isUserOutput(op->out()->global_id())) {
    tensorPool.copyTensorToUserOutput(out, op->out()->global_id());
  } else {
    tensorPool.insert_or_assign(op->out()->global_id(), out);
  }
}

} // namespace tt::runtime::ttnn::operations::layout
