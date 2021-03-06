// Copyright 2017 Yahoo Holdings. Licensed under the terms of the Apache 2.0 license. See LICENSE in the project root.
#include <vespa/log/log.h>
LOG_SETUP("phrasesplitter_test");
#include <vespa/vespalib/testkit/testapp.h>

#include <iomanip>
#include <iostream>
#include <vespa/searchlib/fef/matchdatalayout.h>
#include <vespa/searchlib/fef/phrasesplitter.h>
#include <vespa/searchlib/fef/test/queryenvironment.h>

namespace search {
namespace fef {

class Benchmark : public vespalib::TestApp
{
private:
    FastOS_Time _timer;
    double      _sample;

    void start() { _timer.SetNow(); }
    void sample() { _sample = _timer.MilliSecsToNow(); }
    void run(size_t numRuns, size_t numPositions);

public:
    Benchmark() : _timer(), _sample(0) {}
    int Main() override;
};

void
Benchmark::run(size_t numRuns, size_t numPositions)
{
    test::QueryEnvironment qe;
    std::vector<SimpleTermData> &terms = qe.getTerms();
    MatchDataLayout mdl;
    terms.push_back(SimpleTermData());
    terms.back().setUniqueId(1);
    terms.back().setPhraseLength(3); // phrase with 3 terms
    terms.back().addField(0).setHandle(mdl.allocTermField(0));
    MatchData::UP md = mdl.createMatchData();
    TermFieldMatchData *tmd = md->resolveTermField(terms[0].lookupField(0)->getHandle());
    for (size_t i = 0; i < numPositions; ++i) {
        tmd->appendPosition(TermFieldMatchDataPosition(0, i, 0, numPositions));
    }

    PhraseSplitter ps(qe, 0);

    std::cout << "Start benchmark with numRuns(" << numRuns << ") and numPositions(" << numPositions << ")" << std::endl;

    start();

    ps.bind_match_data(*md);
    for (size_t i = 0; i < numRuns; ++i) {
        ps.update();
    }

    sample();
}

int
Benchmark::Main()
{

    TEST_INIT("benchmark");

    if (_argc != 3) {
        std::cout << "Must specify <numRuns> and <numPositions>" << std::endl;
        return 0;
    }

    size_t numRuns = strtoull(_argv[1], NULL, 10);
    size_t numPositions = strtoull(_argv[2], NULL, 10);

    run(numRuns, numPositions);

    std::cout << "TET:  " << _sample << " (ms)" << std::endl;
    std::cout << "ETPD: " << std::fixed << std::setprecision(10) << _sample / numRuns << " (ms)" << std::endl;

    TEST_DONE();
}

}
}

TEST_APPHOOK(search::fef::Benchmark);
