---
layout: posts
title: "GTest + GMock for Library Tests"
date: "2014-08-02"
categories: testing
---

# What is it

Google Test or GTest is test harness written for C and C++.  It incorporates some pretty nice filters including:

* Out of order testing
* Fast suite development (no header files needed)
* Filtering
* Test output in standard formats for buid systems (like junit test results)

GTest also works very well with Google Mock.

# References
* [Google Test](https://code.google.com/p/googletest/)
* [Google Mock](https://code.google.com/p/googlemock/)

# Introduction of libraries_test

The unit test harness is built in native code and is located along-side the `libraries` as `libraries_test`. The folder structure inside `libraries_test` mirrors the folder structure under test in `libraries`.

The makefile I wrote tries not to leverage any the mk files used in ardupilot. Those are pretty complex and hard to manage and I didn't want to create coupling between makefiles used for artifact build and those used for the test harness.

## Nomenclature

Test suite names are always prefixed with the library under test and then the function (member function) name. I do not mix functional tests in a grab bag style suite because it becomes a mess. If I want to find a test suite, I know that it's the class + function name.  Simple.

Other file naming rules:

* _IT post-fix are for integration tests.
* _UC_ is for use-case tests which involve a sequence of components.

## General policies for unit tests

Each unit test tests a single assert. Test should <strong>not</strong> attempt to assert more than one test condition. Doing so leads to tests that are messy, unmanageable, and lazy.

My Rules:

* Test names are explicit. They list what they are trying to do and what they expect.
* PEC test are parameter enforcement checks.
* BAT are tests that are basic acceptance tests.
* Do not disable tests unless it's a work in progress.
* Do not fix expectations unless you are sure that expectation is correct. Too often I see developers hammering in "fixes" to tests without paying attention to the test scenario.
* Assert what is important.  Do not assert control flow or function calls that are not important.  Conversely, enforce sequence or calls if the calls must be made and/or in a particular order.  If a test over asserts, then the test becomes brittle and the developer will have to constantly maintain the overly restrictive asserts.

## All interactions are MOCKED

If you don't know how to mock this is a good time to learn. I cannot emphasize enough that a good developer is not judged by how well he/she can code but how well he/she can test and designing code that can be tested.  Mocking is once of the most important tools in building great, testable, code.

Using mock you can inject complete control over the code's execution path and decision making. More complex mocks can even simulate sensor data.

In prior projects I've written mocks than fake out file i/o which allowed me to simulate any binary data as if it came from the file system. This allowed me to test conditions I would normally have to create a real file for and use real file i/o.

# How to use

To build:

```
make
```

To run:

```
make run
```
