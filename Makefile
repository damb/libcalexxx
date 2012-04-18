# this is <Makefile>
# ----------------------------------------------------------------------------
# $Id$
# Copyright (c) 2012 by Daniel Armbruster 
# ----
# This file is part of libcalexxx.
# 
# libcalexxx is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# libcalexxx is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with libcalexxx.  If not, see <http://www.gnu.org/licenses/>.
# ----
# Daniel Armbruster 14/03/2012
# 
# REVISIONS and CHANGES
# 14/03/2012	V0.1	Daniel Armbruster (basically taken of liboptimizexx)
#
# ----------------------------------------------------------------------------
#

CALEXCONFIG=STS2

.PHONY: all
all: install doc 

.PHONY: doc
doc: doxydoc

# test programs are placed in a subdirectory
TESTS=$(wildcard tests/*.cc)

.PHONY: tests
tests: reinstall $(patsubst %.cc,%,$(TESTS))

LIBRARIES=libcalexxx.a libcalexxx.so

.PHONY: install
install: $(addprefix $(LOCLIBDIR)/,$(LIBRARIES))
$(LOCLIBDIR)/%: install-include %
	mkdir -pv $(LOCLIBDIR)
	/bin/mv -fv $(word 2,$^) $(LOCLIBDIR)

# ============================================================================
# a variable definition to check variable settings
# ------------------------------------------------

CHECKVAR=$(if $($(1)),,$(error ERROR: missing variable $(1)))
CHECKVARS=$(foreach var,$(1),$(call CHECKVAR,$(var)))

# check for required variables
$(call CHECKVARS,LOCINCLUDEDIR LOCLIBDIR TF_WWWBASEDIR)
$(call CHECKVARS, BOOST_FILESYSTEM_VERSION)

#======================================================================
# files and paths
# ---------------

# all header files used within this project
HEADERS=$(shell find . -name \*.h)
# all source code to be compiled to object files and to be included
# in the binary version of the library
# (see below for the configuration of a preinstantiated version of template
# code)
SRC=$(wildcard *.cc) 

# place where we will copy header files
INCINSTALLPATH=$(LOCINCLUDEDIR)/calexxx
# place where we will copy the binary library
LIBINSTALLPATH=$(LOCLIBDIR)

# name of headers with comments stripped off (these are linked to your include
# directory)
STRIPHEADER=$(addsuffix .strip,$(notdir $(HEADERS)))
# name of installed (exported) header files (these are the names in your
# include directory)
INSTHEADER=$(addprefix $(INCINSTALLPATH)/,$(filter-out ./tests/%,$(HEADERS)))

# README files
README=README.doxydoc

#======================================================================
# comments can be removed from headers
# ------------------------------------
 
# if defined, empty lines are kept in comment-stripped headers
# to synchronize line numbers (necessary during library debugging)
EMPTYPRINT=1
#EMPTYPRINT=0

# define this to be cat in case you do not have remcmmnt available
ifndef TF_REMCMMNT
TF_REMCMMNT=cat
endif
#REMCMMNT=remcmmnt
#REMCMMNT=cat

#======================================================================
# compiler and preprocessor flags
# -------------------------------
FLAGS= -D$(CALEXCONFIG) 
FLAGS+= -DBOOST_FILESYSTEM_VERSION=$(BOOST_FILESYSTEM_VERSION)
FLAGS+=$(MYFLAGS) -fPIC -march=native -O2 -fno-reorder-blocks \
			 -fno-reorder-functions -pipe -std=c++0x
CXXFLAGS+=-Wall $(FLAGS)
LDFLAGS=$(addprefix -L,$(LOCLIBDIR))
CPPFLAGS=$(addprefix -I,$(LOCINCLUDEDIR)) $(FLAGS)

#======================================================================
# targets
# -------

# files which are to be edited
flist: Makefile tests/Makefile doxydoc.cfg $(README) \
       $(HEADERS) $(SRC) $(TESTS) $(wildcard doxy*.cfg) \
	echo $(filter-out lib/% tests/%,$^) | tr ' ' '\n' | sort > $@
	echo $(filter lib/%,$^) | tr ' ' '\n' | sort >> $@
	echo $(filter tests/%,$^) | tr ' ' '\n' | sort >> $@

# Offers you a list of files to edit (ask Thomas for appropriate vim
# configuration).
.PHONY: edit
edit: flist; vim $<

# Target clean removes everything except the source code, headers and
# documentation. (See also doxyclean)
.PHONY: clean
clean: ; 
	-find . -name \*.bak | xargs --no-run-if-empty /bin/rm -v
	-find . -name \*.o | xargs --no-run-if-empty /bin/rm -v
	-find . -name \*.d | xargs --no-run-if-empty /bin/rm -v
	-find . -name \*.h.strip | xargs --no-run-if-empty /bin/rm -v
	-/bin/rm -vf flist *.o install-include *.xxx junk* *.a *.so
	-cd tests; $(MAKE) clean

#======================================================================
# library part
# ============
#
# create the binary library
# -------------------------
LIBOBS=$(patsubst %.cc,%.o,$(SRC))

libcalexxx.a: $(INSTHEADER) $(LIBOBS)
	ar rcv $@ $(LIBOBS)
	ranlib $@

libcalexxx.so: $(INSTHEADER) $(LIBOBS)
	$(CXX) $(CXXFLAGS) -shared -o $@ $(LIBOBS)

#======================================================================
# dependencies
# ------------
#
# The compiler is used to create dependency files, which are included below.

%.d: %.cc
	$(SHELL) -ec '$(CXX) -M $(CPPFLAGS) $< \
      | sed '\''s,\($(notdir $*)\)\.o[ :]*,$(dir $@)\1.o $@ : ,g'\'' \
        > $@; \
      [ -s $@ ] || rm -f $@'

include $(patsubst %.cc,%.d,$(SRC)) 

#======================================================================
# header files
# ------------
#
# Since we extensively document the class structures within the header files,
# these should be stripped for the production version (otherwise the compiler
# has to scan all comments on each compile). Stripping is done by the rule
# below and is controlled by the variables TF_REMCMMNT and EMPTYPRINT (see
# above).

# comment stripping
# -----------------
$(call CHECKVARS,TF_REMCMMNT EMPTYPRINT)

# awk rule assumes that the first non-comment line starts with '#'
# and that the first (copyright) comment end with pattern "^ */"
%.h.strip: %.h
	awk 'BEGIN {hot=1;} /^ \*\// { if (hot) { hot=2; print; next;} }\
        /^#/ { hot=0; } \
        { if (hot==2) { print ""; } else if (hot) { print; } }' $< > $@
	$(TF_REMCMMNT) $< | awk 'BEGIN {hot=0;} \
        /^ *$$/ { if ((hot) && ($(EMPTYPRINT))) { print ""; } next; } \
        /^#/ { hot=1; } { if (hot) print; }' >> $@

#----------------------------------------------------------------------
.PRECIOUS: %.h.strip
      
$(call CHECKVAR,INCINSTALLPATH)
$(INCINSTALLPATH)/%.h: %.h.strip
	mkdir -vp $(dir $@)
	-rm -fv $@
	/bin/cp -vpd $< $@

# install header files
.PHONY: install-include
install-include: $(INSTHEADER)

#======================================================================
# reinstall target
# is necessary in case of header file problems (e.g. remcmmnt not installed)
.PHONY: clean-include
clean-include: 
	/bin/rm -fv $(INSTHEADER)

.PHONY: reinstall
reinstall:
	$(MAKE) clean
	$(MAKE) clean-include
	$(MAKE) install

#======================================================================
# documentation part
# ------------------
#
# targets commonly used:
# ----------------------
#
# make doxyclean        removes all documentation
# make doxydoc          creates doxygen documentation in the DOXYWWWPATH
# make doxyview         creates doxygen documentation and launches the browser
# 											to browse in the documentation
# make doxyconf         edit the doxygen configuration file
#
# If you launch "make doxydoc" the documentation will be written to
# DOXYWWWPATH (see below). This is meant to export the documentation through
# your homepage. The doxyfull directory is just a symbolic link to this
# directory.
#

$(call CHECKVARS,TF_WWWBASEDIR TF_BROWSER)

DOXYWWWPATH=$(TF_WWWBASEDIR)/libcalexxx

.PHONY: doxyclean doxyview doxydoc doxyconf

doxyclean: ;/bin/rm -rfv $(TF_WWWBASEDIR)/libcalexxx doxydoc.xxx

DOXYSRC=$(README) $(HEADERS) $(SRC) 
#  tests/f77procs.P tests/f77procs.f \
#  tests/f77common.inc tests/f77common_com.P

# create doxygen intermediate configuration
PWD=$(shell env pwd)
doxydoc.xxx: doxydoc.cfg
	sed 's,<OUTPUTDIRECTORY>,$(DOXYWWWPATH),g;s,<STRIPFROMPATH>,$(PWD),g' \
	  $< > $@
# create commented version of doxygen configuration
doxycomm.xxx: doxydoc.cfg
	/bin/cp -vf $< $@; doxygen -u $@

$(DOXYWWWPATH)/html/index.html: doxydoc.xxx $(DOXYSRC)
	mkdir -vp $(DOXYWWWPATH)
	doxygen $<

doxydoc: $(DOXYWWWPATH)/html/index.html

doxyview: $(DOXYWWWPATH)/html/index.html
	$(TF_BROWSER) file:$< &

#======================================================================
# delegate test targets
# ---------------------
#
tests/%: tests/%.cc install
	@cd tests; echo "#############################"; $(MAKE) -s $(notdir $@)


# ----- END OF Makefile -----
