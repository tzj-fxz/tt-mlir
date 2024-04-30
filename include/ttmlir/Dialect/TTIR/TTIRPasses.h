// SPDX-FileCopyrightText: © 2024 Tenstorrent Inc.
//
// SPDX-License-Identifier: Apache-2.0

#ifndef TTMLIR_TTMLIR_DIALECT_TTIR_TTIRPASSES_H
#define TTMLIR_TTMLIR_DIALECT_TTIR_TTIRPASSES_H

#include "mlir/Pass/Pass.h"
#include "ttmlir/Dialect/TTIR/TTIRDialect.h"
#include "ttmlir/Dialect/TTIR/TTIROps.h"
#include <memory>

namespace mlir::tt::ttir {
#define GEN_PASS_DECL
#include "ttmlir/Dialect/TTIR/TTIRPasses.h.inc"

#define GEN_PASS_REGISTRATION
#include "ttmlir/Dialect/TTIR/TTIRPasses.h.inc"
} // namespace mlir::tt::ttir

#endif
