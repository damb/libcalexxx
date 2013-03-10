# this is <Makefile>
# ----------------------------------------------------------------------------
# $Id$
# Copyright (c) 2012 by Daniel Armbruster 
# ----
# This file is part of liboptimizexx.
# 
# liboptimizexx is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# liboptimizexx is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with liboptimizexx.  If not, see <http://www.gnu.org/licenses/>.
# ----
# Daniel Armbruster 15/03/2012
# 
# REVISIONS and CHANGES
# 15/03/2012		V0.1		Daniel Armbruster
# 04/06/2012  	V0.2  	added calexOutFileParser
# 08/06/2012  	V0.3  	added calexParamFileGen
#
# ----------------------------------------------------------------------------
CPPFLAGS=-I$(LOCALINCLUDEDIR) 
LDFLAGS=-L$(LOCALLIBDIR) 

STANDARDTEST= calexParamTest calexResultTest commandlineParserTest 
PROGRAMS= calexOutFileParser calexParamFileGen

.PHONY: install
install: $(addprefix $(LOCALBINDIR)/,$(PROGRAMS))
$(LOCALBINDIR)/%: %
	mkdir -pv $(LOCALBINDIR)
	/bin/mv -fv $< $(LOCALBINDIR)

clean:
	-find . -name \*.o | xargs --no-run-if-empty /bin/rm -v
	-/bin/rm -v $(STANDARDTEST)
	-/bin/rm -v $(PROGRAMS)

# =============================================================================
#
CHECKVAR=$(if $($(1)),,$(error ERROR: missing variable $(1)))
CHECKVARS=$(foreach var,$(1),$(call CHECKVAR,$(var)))
#
$(call CHECKVARS,LOCALINCLUDEDIR LOCALLIBDIR LOCALBINDIR)
$(call CHECKVARS, BOOST_FILESYSTEM_VERSION)

FLAGS= -DBOOST_FILESYSTEM_VERSION=$(BOOST_FILESYSTEM_VERSION)
FLAGS += $(MYFLAGS) -std=c++0x
CFLAGS += -march=native -O2 -fno-reorder-blocks -fno-reorder-functions -pipe

CXXFLAGS += -Wall $(FLAGS)
LDFLAGS+=$(addprefix -L,$(LOCALLIBDIR))
CPPFLAGS+=$(addprefix -I,$(LOCALINCLUDEDIR)) $(FLAGS)

# ----------------------------------------------------------------------------

$(addsuffix .o,$(STANDARDTEST) calexOutFileParser): %.o: %.cc
	@$(CXX) -c -o $@ $< $(CXXFLAGS) $(CPPFLAGS) $(FLAGS) -std=c++0x

$(STANDARDTEST): %: %.o 	
	@echo -e "\n[ Compiling test program: $@ ]\n"	
	@$(CXX) -o $@ $< $(LDFLAGS) -lcalexxx -std=c++0x

calexParamFileGen calexOutFileParser: %: %.o
	@echo -e "\n[ Compiling test program: $@ ]\n"	
	$(CXX) -o $@ $^ -I$(LOCALINCLUDEDIR) -lcalexxx -lboost_filesystem \
	-lboost_program_options -L$(LOCALLIBDIR) $(CXXFLAGS) $(FLAGS) $(LDFLAGS)

# ----- END OF Makefile -----
