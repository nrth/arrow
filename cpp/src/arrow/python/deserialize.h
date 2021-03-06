// Licensed to the Apache Software Foundation (ASF) under one
// or more contributor license agreements.  See the NOTICE file
// distributed with this work for additional information
// regarding copyright ownership.  The ASF licenses this file
// to you under the Apache License, Version 2.0 (the
// "License"); you may not use this file except in compliance
// with the License.  You may obtain a copy of the License at
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing,
// software distributed under the License is distributed on an
// "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
// KIND, either express or implied.  See the License for the
// specific language governing permissions and limitations
// under the License.

#ifndef ARROW_PYTHON_ARROW_TO_PYTHON_H
#define ARROW_PYTHON_ARROW_TO_PYTHON_H

#include <cstdint>
#include <memory>
#include <vector>

#include "arrow/python/serialize.h"
#include "arrow/status.h"
#include "arrow/util/visibility.h"

namespace arrow {

class RecordBatch;
class Tensor;

namespace io {

class RandomAccessFile;

}  // namespace io

namespace py {

/// \brief Read serialized Python sequence from file interface using Arrow IPC
/// \param[in] src a RandomAccessFile
/// \param[out] out the reconstructed data
/// \return Status
ARROW_EXPORT
Status ReadSerializedObject(io::RandomAccessFile* src, SerializedPyObject* out);

/// \brief Reconstruct SerializedPyObject from representation produced by
/// SerializedPyObject::GetComponents.
///
/// \param[in] num_tensors number of tensors in the object
/// \param[in] num_buffers number of buffers in the object
/// \param[in] data a list containing pyarrow.Buffer instances. Must be 1 +
/// num_tensors * 2 + num_buffers in length
/// \param[out] out the reconstructed object
/// \return Status
ARROW_EXPORT
Status GetSerializedFromComponents(int num_tensors, int num_buffers, PyObject* data,
                                   SerializedPyObject* out);

/// \brief Reconstruct Python object from Arrow-serialized representation
/// \param[in] context Serialization context which contains custom serialization
/// and deserialization callbacks. Can be any Python object with a
/// _serialize_callback method for serialization and a _deserialize_callback
/// method for deserialization. If context is None, no custom serialization
/// will be attempted.
/// \param[in] object Object to deserialize
/// \param[in] base a Python object holding the underlying data that any NumPy
/// arrays will reference, to avoid premature deallocation
/// \param[out] out The returned object
/// \return Status
/// This acquires the GIL
ARROW_EXPORT
Status DeserializeObject(PyObject* context, const SerializedPyObject& object,
                         PyObject* base, PyObject** out);

/// \brief Reconstruct Tensor from Arrow-serialized representation
/// \param[in] object Object to deserialize
/// \param[out] out The deserialized tensor
/// \return Status
ARROW_EXPORT
Status DeserializeTensor(const SerializedPyObject& object, std::shared_ptr<Tensor>* out);

ARROW_EXPORT
Status ReadTensor(std::shared_ptr<Buffer> src, std::shared_ptr<Tensor>* out);

}  // namespace py
}  // namespace arrow

#endif  // ARROW_PYTHON_ARROW_TO_PYTHON_H
