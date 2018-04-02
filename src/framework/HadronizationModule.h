/*******************************************************************************
 * Copyright (c) The JETSCAPE Collaboration, 2017
 *
 * For the full list of contributors see AUTHORS.
 * Report issues at https://github.com/amajumder/JETSCAPE-COMP/issues
 * or via email to bugs.jetscape.org@gmail.com
 *
 * Distributed under the GNU General Public License 3.0 (GPLv3 or later).
 * See COPYING for details.
 ******************************************************************************/


#ifndef HADRONIZATIONMODULE_H
#define HADRONIZATIONMODULE_H

#include "Hadronization.h"

namespace Jetscape {

template <typename Derived>
class HadronizationModule : public Hadronization
{
  
 public:

  using Hadronization::Hadronization;
  
  virtual shared_ptr<Hadronization> Clone() const override
   {
     auto ret = make_shared<Derived>(static_cast<const Derived&>(*this));
     return ret;
   }  
};


} // end namespace Jetscape


#endif
