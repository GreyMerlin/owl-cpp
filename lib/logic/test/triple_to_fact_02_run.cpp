/** @file "/owlcpp/lib/logic/test/triple_to_fact_02_run.cpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#define BOOST_TEST_MODULE triple_to_fact_02_run
#include "boost/test/unit_test.hpp"
#include "test/exception_fixture.hpp"
#include "test/sample_data.hpp"
#include "owlcpp/rdf/triple_store.hpp"
#include "owlcpp/io/input.hpp"
#include "owlcpp/logic/triple_to_fact.hpp"
#include "factpp/Kernel.hpp"

namespace owlcpp{ namespace test{

BOOST_GLOBAL_FIXTURE( Exception_fixture );

/**
*******************************************************************************/
BOOST_AUTO_TEST_CASE( test_data_property_assertion ) {
   Triple_store ts;
   load_file(sample_file_path("data_property_assertion_01.owl"), ts);
   ReasoningKernel k;
   submit_triples(ts, k, false);
}

}//namespace test
}//namespace owlcpp