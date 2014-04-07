// Copyright 2014 Google Inc. All rights reserved.
//
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file or at
// https://developers.google.com/open-source/licenses/bsd

#include "media/formats/mp4/chunk_info_iterator.h"

#include "base/logging.h"
#include "base/memory/scoped_ptr.h"
#include "testing/gtest/include/gtest/gtest.h"

namespace {
struct ChunkProperty {
  uint32 samples_per_chunk;
  uint32 sample_description_index;
};
}  // namespace

namespace media {
namespace mp4 {

const uint32 kNumChunks = 100;
const ChunkInfo kChunkInfos[] = {
    {1, 8, 1}, {9, 5, 1}, {25, 7, 2}, {48, 63, 2}, {80, 2, 1}};

class ChunkInfoIteratorTest : public testing::Test {
 public:
  ChunkInfoIteratorTest() {
    // Build chunk info table from kChunkInfos.
    uint32 length = sizeof(kChunkInfos) / sizeof(ChunkInfo);
    CHECK(kChunkInfos[0].first_chunk == 1);
    CHECK(kChunkInfos[length - 1].first_chunk <= kNumChunks);
    uint32 chunk_index = kChunkInfos[0].first_chunk;
    for (uint32 i = 0; i < length; ++i) {
      uint32 next_first_chunk =
          (i == length - 1) ? kNumChunks + 1 : kChunkInfos[i + 1].first_chunk;
      for (; chunk_index < next_first_chunk; ++chunk_index) {
        ChunkProperty chunk = {kChunkInfos[i].samples_per_chunk,
                               kChunkInfos[i].sample_description_index};
        chunk_info_table_.push_back(chunk);
      }
    }

    sample_to_chunk_.chunk_info.assign(kChunkInfos, kChunkInfos + length);
    chunk_info_iterator_.reset(new ChunkInfoIterator(sample_to_chunk_));
  }

 protected:
  std::vector<ChunkProperty> chunk_info_table_;
  SampleToChunk sample_to_chunk_;
  scoped_ptr<ChunkInfoIterator> chunk_info_iterator_;

 private:
  DISALLOW_COPY_AND_ASSIGN(ChunkInfoIteratorTest);
};

TEST_F(ChunkInfoIteratorTest, EmptyChunkInfo) {
  SampleToChunk sample_to_chunk;
  ChunkInfoIterator iterator(sample_to_chunk);
  EXPECT_FALSE(iterator.IsValid());
  EXPECT_EQ(0u, iterator.LastFirstChunk());
}

TEST_F(ChunkInfoIteratorTest, LastFirstChunk) {
  ASSERT_EQ(
      kChunkInfos[sizeof(kChunkInfos) / sizeof(ChunkInfo) - 1].first_chunk,
      chunk_info_iterator_->LastFirstChunk());
}

TEST_F(ChunkInfoIteratorTest, NumSamples) {
  for (uint32 i = 0; i < kNumChunks; ++i) {
    for (uint32 num_samples = 0, j = i; j < kNumChunks; ++j) {
      num_samples += chunk_info_table_[j].samples_per_chunk;
      ASSERT_EQ(num_samples, chunk_info_iterator_->NumSamples(i + 1, j + 1));
    }
  }
}

TEST_F(ChunkInfoIteratorTest, AdvanceChunk) {
  for (uint32 chunk = 0; chunk < kNumChunks; ++chunk) {
    ASSERT_TRUE(chunk_info_iterator_->IsValid());
    EXPECT_EQ(chunk + 1, chunk_info_iterator_->current_chunk());
    EXPECT_EQ(chunk_info_table_[chunk].samples_per_chunk,
              chunk_info_iterator_->samples_per_chunk());
    EXPECT_EQ(chunk_info_table_[chunk].sample_description_index,
              chunk_info_iterator_->sample_description_index());
    // Will always succeed as Sample to Chunk Box does not define the last
    // chunk.
    ASSERT_TRUE(chunk_info_iterator_->AdvanceChunk());
  }
}

TEST_F(ChunkInfoIteratorTest, AdvanceSample) {
  for (uint32 chunk = 0; chunk < kNumChunks; ++chunk) {
    uint32 samples_per_chunk = chunk_info_table_[chunk].samples_per_chunk;
    for (uint32 sample = 0; sample < samples_per_chunk; ++sample) {
      ASSERT_TRUE(chunk_info_iterator_->IsValid());
      EXPECT_EQ(chunk + 1, chunk_info_iterator_->current_chunk());
      EXPECT_EQ(chunk_info_table_[chunk].samples_per_chunk,
                chunk_info_iterator_->samples_per_chunk());
      EXPECT_EQ(chunk_info_table_[chunk].sample_description_index,
                chunk_info_iterator_->sample_description_index());
      // Will always succeed as Sample to Chunk Box does not define the last
      // chunk.
      ASSERT_TRUE(chunk_info_iterator_->AdvanceSample());
    }
  }
}

}  // namespace mp4
}  // namespace media