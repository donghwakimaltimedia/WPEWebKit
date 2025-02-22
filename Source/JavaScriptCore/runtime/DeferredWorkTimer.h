/*
 * Copyright (C) 2017-2020 Apple Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#pragma once

#include "JSRunLoopTimer.h"
#include "Strong.h"

#include <wtf/Deque.h>
#include <wtf/HashMap.h>
#include <wtf/Lock.h>
#include <wtf/Vector.h>

namespace JSC {

class JSPromise;
class VM;
class JSCell;

class JS_EXPORT_PRIVATE DeferredWorkTimer : public JSRunLoopTimer {
public:
    using Base = JSRunLoopTimer;

    void doWork(VM&) override;

    using Ticket = JSObject*;
    void addPendingWork(VM&, Ticket, Vector<Strong<JSCell>>&& dependencies);
    bool hasPendingWork(Ticket);
    bool hasDependancyInPendingWork(Ticket, JSCell* dependency);
    bool cancelPendingWork(Ticket);

    using Task = Function<void()>;
    void scheduleWorkSoon(Ticket, Task&&);

    void stopRunningTasks() { m_runTasks = false; }

    void runRunLoop();

    static Ref<DeferredWorkTimer> create(VM& vm)
    {
        return adoptRef(*new DeferredWorkTimer(vm));
    }

private:
    DeferredWorkTimer(VM&);

    Lock m_taskLock;
    bool m_runTasks { true };
    bool m_shouldStopRunLoopWhenAllTicketsFinish { false };
    bool m_currentlyRunningTask { false };
    Deque<std::tuple<Ticket, Task>> m_tasks;
    HashMap<Ticket, Vector<Strong<JSCell>>> m_pendingTickets;
};

} // namespace JSC
