// Copyright 2017 Yahoo Holdings. Licensed under the terms of the Apache 2.0 license. See LICENSE in the project root.
#pragma once

#include "tracenode.h"

namespace vespalib {

/**
 * A Trace object contains ad-hoc string notes organized in a strict-loose
 * tree. A Trace object consists of a trace level indicating which trace notes
 * should be included and a TraceTree object containing the tree structure and
 * collecting the trace information. Tracing is used to collect debug
 * information about a Routable traveling through the system. The trace level
 * is in the range [0,9]. 0 means no tracing, and 9 means all tracing is
 * enabled. A client that has the ability to trace information will have a
 * predefined level attached to that information. If the level on the
 * information is lower or equal to the level set in the Trace object, the
 * information will be traced.
 */
class Trace {
private:
    uint32_t  _level;
    TraceNode _root;

public:

    /**
     * Create an empty Trace with level set to 0 (no tracing)
     */
    Trace();
    ~Trace();

    /**
     * Create an empty trace with given level.
     *
     * @param level Level to set.
     */
    explicit Trace(uint32_t level);

    /**
     * Remove all trace information and set the trace level to 0.
     *
     * @return This, to allow chaining.
     */
    Trace &clear();

    /**
     * Swap the internals of this with another.
     *
     * @param other The trace to swap internals with.
     * @return This, to allow chaining.
     */
    Trace &swap(Trace &other);

    /**
     * Set the trace level. 0 means no tracing, 9 means enable all tracing.
     *
     * @param level The level to set.
     * @return This, to allow chaining.
     */
    Trace &setLevel(uint32_t level);

    /**
     * Returns the trace level.
     *
     * @return The trace level.
     */
    uint32_t getLevel() const { return _level; }

    /**
     * Check if information with the given level should be traced. This method
     * is added to allow clients to check if something should be traced before
     * spending time building up the trace information itself.
     *
     * @param level The trace level to test.
     * @return True if tracing is enabled for the given level, false otherwise.
     */
    bool shouldTrace(uint32_t level) const { return level <= _level; }

    /**
     * Add the given note to the trace information if tracing is enabled for
     * the given level. If the addTime parameter is true, then the note is
     * prefixed with the current time. This is the default behaviour when
     * ommiting this parameter.
     *
     * @param level   The trace level of the note.
     * @param note    The note to add.
     * @param addTime Whether or not to prefix note with a timestamp.
     * @return True if the note was added to the trace information, false
     * otherwise.
     */
    bool trace(uint32_t level, const string &note, bool addTime = true);

    /**
     * Returns the root of the trace tree.
     *
     * @return The root.
     */
    TraceNode &getRoot() { return _root; }

    /**
     * Returns a const reference to the root of the trace tree.
     *
     * @return The root.
     */
    const TraceNode &getRoot() const { return _root; }

    /**
     * Returns a string representation of the contained trace tree. This is a
     * readable, non-parseable string.
     *
     * @return Readable trace string.
     */
    string toString() const { return _root.toString(31337); }
};

#define VESPALIB_TRACE2(ttrace, level, note, addTime) \
    if (ttrace.shouldTrace(level)) {             \
        ttrace.trace(level, note, addTime);      \
    }

#define VESPALIB_TRACE(trace, level, note) \
    VESPALIB_TRACE2(trace, level, note, true)

} // namespace vespalib

