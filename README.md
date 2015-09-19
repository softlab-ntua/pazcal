Pazcal
======

**Pazcal** is an educational subset of the C programming language.
It is intended for teaching 
[Introduction to Computer Programming]
(http://courses.softlab.ntua.gr/progintro/)
to first-year students of the
[School of Electrical and Computer Engineering](http://www.ece.ntua.gr/)
at the
[National Technical University of Athens](http://www.ntua.gr/).
It comes with a set of notes, written by
[Stathis Zachos](http://www.corelab.ece.ntua.gr/~zachos/) and
[Nikos Papaspyrou](http://www.softlab.ntua.gr/~nickie/),
which will eventually be added to this repository.

Pazcal is actually:

* A header file, called `pazcal.h`, defining a set of C macros that
  is meant to facilitate first-year students learning C.
  All these macros are written in `UPPERCASE` letters, so they can be
  easily distinguished from things existing in regular C.
* A lint-like tool, called `pzcheck` that forbids everything in C that
  should not be taught to first-year students.
* A script, called `pzc` that combines the above two and can be
  considered as an implementation of the language.

For the time being, `pzcheck` does not do anything: it allows all of C
plus the macros defined in `pazcal.h`.


Installation
------------

To install Pazcal to your computer, assuming you are running
Linux or Mac OS, or that you know what to do with your Windows:

0. Make sure you have `gcc` installed and working.

1. Copy this directory to, say, `/full/path/pazcal`.

2. Edit `/full/path/pazcal/bin/pzc` and set `PAZCAL_DIR` to
   point to `/full/path/pazcal`.

3. Make sure that all files in `/full/path/pazcal/bin` are
   executable and that this directory is in your `PATH`.


Maintainer
----------

The implementation of Pazcal is maintained by

* [Nikos Papaspyrou](http://www.softlab.ntua.gr/~nickie/)
   \<<nickie@softlab.ntua.gr>\>
