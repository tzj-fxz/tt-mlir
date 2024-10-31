// SPDX-FileCopyrightText: (c) 2024 Tenstorrent AI ULC
//
// SPDX-License-Identifier: Apache-2.0

#ifndef TTMLIR_DIALECT_TTNN_ANALYSIS_MEMORYLAYOUTANALYSIS_H
#define TTMLIR_DIALECT_TTNN_ANALYSIS_MEMORYLAYOUTANALYSIS_H

#include "mlir/Dialect/Func/IR/FuncOps.h"
#include "ttmlir/Dialect/TTNN/Analysis/Edge.h"
#include "ttmlir/Dialect/TTNN/Analysis/L1ChainConfig.h"
#include "ttmlir/Dialect/TTNN/Analysis/TTNNAnalysis.h"

namespace mlir::tt::ttnn {

enum class MemoryLayoutAnalysisPolicyType {
  DFSharding,
};

struct MemoryLayoutAnalysisInput {
  llvm::DenseMap<Operation *, std::vector<tt::LayoutAttr>> legalLayouts;
  unsigned usableL1CacheSize = 0;
  std::unordered_set<Edge> overrideReshardEdges;

  MemoryLayoutAnalysisInput() : legalLayouts() {}

  MemoryLayoutAnalysisInput(
      const llvm::DenseMap<Operation *, std::vector<tt::LayoutAttr>>
          &legalLayouts,
      unsigned usableL1CacheSize,
      const std::unordered_set<Edge> &overrideReshardEdges)
      : legalLayouts(legalLayouts), usableL1CacheSize(usableL1CacheSize),
        overrideReshardEdges(overrideReshardEdges) {}

  bool operator==(const MemoryLayoutAnalysisInput &rhs) const {
    return legalLayouts == rhs.legalLayouts;
  }

  bool operator!=(const MemoryLayoutAnalysisInput &rhs) const {
    return !(*this == rhs);
  }
};

struct MemoryLayoutAnalysisResult {
  llvm::DenseMap<Operation *, std::vector<tt::LayoutAttr>> legalLayouts;
  std::unordered_set<Edge> memReconfigEdges;
  llvm::DenseMap<func::FuncOp, llvm::SmallVector<Operation *>> schedule;

  MemoryLayoutAnalysisResult()
      : legalLayouts(), memReconfigEdges(), schedule() {}

  MemoryLayoutAnalysisResult(
      const llvm::DenseMap<Operation *, std::vector<tt::LayoutAttr>>
          &legalLayouts,
      const std::unordered_set<Edge> &memReconfigEdges)
      : legalLayouts(legalLayouts), memReconfigEdges(memReconfigEdges) {}
};

// Analyze and determine which parts of the model graph can be pushed to L1
// memory. Produce L1 chain configs consisting of OPs leaving results in L1.
//
class MemoryLayoutAnalysis : public TTNNAnalysis<MemoryLayoutAnalysisInput,
                                                 MemoryLayoutAnalysisResult> {

private:
  void analysisImplementation() override;
  bool applyOverrides() override;
  std::vector<L1ChainConfig> l1ChainConfigs;

public:
  MemoryLayoutAnalysis(Operation *op) : TTNNAnalysis(op) {}
};
} // namespace mlir::tt::ttnn

#endif // TTMLIR_DIALECT_TTNN_ANALYSIS_MEMORYLAYOUTANALYSIS_H