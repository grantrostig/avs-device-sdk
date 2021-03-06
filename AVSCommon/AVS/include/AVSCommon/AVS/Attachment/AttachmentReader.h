/*
 * AttachmentReader.h
 *
 * Copyright 2017 Amazon.com, Inc. or its affiliates. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License").
 * You may not use this file except in compliance with the License.
 * A copy of the License is located at
 *
 *     http://aws.amazon.com/apache2.0/
 *
 * or in the "license" file accompanying this file. This file is distributed
 * on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
 * express or implied. See the License for the specific language governing
 * permissions and limitations under the License.
 */

#ifndef ALEXA_CLIENT_SDK_AVS_COMMON_AVS_INCLUDE_AVS_COMMON_AVS_ATTACHMENT_ATTACHMENT_READER_H_
#define ALEXA_CLIENT_SDK_AVS_COMMON_AVS_INCLUDE_AVS_COMMON_AVS_ATTACHMENT_ATTACHMENT_READER_H_

#include <chrono>
#include <cstddef>

namespace alexaClientSDK {
namespace avsCommon {
namespace avs {
namespace attachment {

/**
 * A class that provides functionality to read data from an @c Attachment.
 */
class AttachmentReader {
public:
    /**
     * An enum class to allow configuration of the type of reader.
     */
    enum class Policy {
        /// A read of n bytes will not return until n bytes are available, or a timeout occurs.
        BLOCKING,
        /// A read of n bytes will return immediately, whether n bytes were available or not.
        NON_BLOCKING
    };

    /**
     * An enum class to communicate the possible states following a @c read() call.
     */
    enum class ReadStatus {
        /// Things are ok.
        OK,
        /// On a request for n bytes, less than n bytes were available on a non-blocking read.
        OK_WOULDBLOCK,
        /// On a request for n bytes, less than n bytes were available on a blocking read.
        OK_TIMEDOUT,
        /// The underlying data representation is no longer readable.
        CLOSED,
        /// The writer has corrupted the reader data.  The attachment is no longer valid.
        ERROR_OVERRUN,
        /// The number of bytes in the request is smaller than the word-size of the underlying data representation.
        ERROR_BYTES_LESS_THAN_WORD_SIZE,
        /// A non-specified error occurred.
        ERROR_INTERNAL
    };

    /// An enum class to indicate when the @c read() function should stop returning data after a call to @c close().
    enum class ClosePoint {
        /// Stop returning data immediately after the @c close() call.
        IMMEDIATELY,
        /// Stop returning data when all of the data in the buffer at the time @c close() was called has been read.
        AFTER_DRAINING_CURRENT_BUFFER
    };

    /*
     * Destructor.
     */
    virtual ~AttachmentReader() = default;

    /**
     * The read function.
     *
     * @param buf The buffer where data should be copied to.
     * @param numBytes The size of the buffer in bytes.
     * @param[out] readStatus The out-parameter where the resulting state of the read will be expressed.
     * @param timeoutMs The timeout for this read call in milliseconds.  This value is only used for the @c BLOCKING
     * reader policy.
     * @return The number of bytes read as a result of this call.
     */
    virtual std::size_t read(void* buf, std::size_t numBytes, ReadStatus* readStatus,
                             std::chrono::milliseconds timeoutMs = std::chrono::milliseconds(0)) = 0;

    /**
     * The close function.  An implementation will take care of any resource management when a reader no longer
     * needs to use an attachment.
     *
     * @param closePoint The point at which the reader should stop reading from the attachment.
     */
    virtual void close(ClosePoint closePoint = ClosePoint::AFTER_DRAINING_CURRENT_BUFFER) = 0;
};

} // namespace attachment
} // namespace avs
} // namespace avsCommon
} // namespace alexaClientSDK

#endif // ALEXA_CLIENT_SDK_AVS_COMMON_AVS_INCLUDE_AVS_COMMON_AVS_ATTACHMENT_ATTACHMENT_READER_H_
