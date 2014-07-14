/*
 * Copyright © 2012 Intel Corporation
 * All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 *
 * Authors:
 *    Jackie Li <yaodong.li@intel.com>
 *
 */
#ifndef HDCP_CONTROL_H
#define HDCP_CONTROL_H

#include <IHdcpControl.h>
#include <common/base/SimpleThread.h>

namespace android {
namespace intel {

class HdcpControl : public IHdcpControl {
public:
    HdcpControl();
    virtual ~HdcpControl();

public:
    virtual bool startHdcp();
    virtual bool startHdcpAsync(HdcpStatusCallback cb, void *userData);
    virtual bool stopHdcp();

protected:
    bool enableAuthentication();
    bool disableAuthentication();
    bool enableOverlay();
    bool disableOverlay();
    bool enableDisplayIED();
    bool disableDisplayIED();
    bool isHdcpSupported();
    bool checkAuthenticated();
    virtual bool preRunHdcp();
    virtual bool postRunHdcp();
    bool runHdcp();
    inline void signalCompletion();

private:
    enum {
        HDCP_INLOOP_RETRY_NUMBER = 1,
        HDCP_INLOOP_RETRY_DELAY_US = 50000,
        HDCP_VERIFICATION_DELAY_MS = 2000,
        HDCP_ASYNC_START_DELAY_MS = 100,
        HDCP_AUTHENTICATION_SHORT_DELAY_MS = 200,
        HDCP_AUTHENTICATION_LONG_DELAY_MS = 2000,
        HDCP_AUTHENTICATION_TIMEOUT_MS = 5000,
        HDCP_RETRY_LIMIT = 10,
    };

    enum {
        CALLBACK_PENDING,
        CALLBACK_AUTHENTICATED,
        CALLBACK_NOT_AUTHENTICATED,
    };

protected:
    HdcpStatusCallback mCallback;
    void *mUserData;
    int mCallbackState;
    Mutex mMutex;
    Condition mStoppedCondition;
    Condition mCompletedCondition;
    bool mWaitForCompletion;
    bool mStopped;
    bool mAuthenticated;
    int mActionDelay;  // in milliseconds
    uint32_t mAuthRetryCount;

private:
    DECLARE_THREAD(HdcpControlThread, HdcpControl);
};

} // namespace intel
} // namespace android

#endif /* HDCP_CONTROL_H */
