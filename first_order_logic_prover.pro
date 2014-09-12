TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
QMAKE_CXXFLAGS += -std=c++1y
SOURCES += main.cpp
LIBS += -lboost_unit_test_framework
HEADERS += \
    first_order_logic.hpp \
    function.hpp \
    predicate.hpp \
    proof_tree.hpp \
    term_generator.hpp \
    test.hpp \
    gentzen_system.hpp \
    substitution.hpp \
    term.hpp \
    variable.hpp \
    constant.hpp \
    definite_clause.hpp \
    sentence.hpp \
    knowledge_base.hpp \
    praser.hpp

OTHER_FILES += \
    theorem_prover.pro.user \
    LICENSE \
    README.md

PRECOMPILED_HEADER = $$HEADERS
