#include "DataFormats/Common/interface/Handle.h"

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "BEAN/Collections/interface/BNbxlumi.h"
#include "BEAN/Collections/interface/BNelectron.h"
#include "BEAN/Collections/interface/BNevent.h"
#include "BEAN/Collections/interface/BNjet.h"
#include "BEAN/Collections/interface/BNmcparticle.h"
#include "BEAN/Collections/interface/BNmet.h"
#include "BEAN/Collections/interface/BNmuon.h"
#include "BEAN/Collections/interface/BNphoton.h"
#include "BEAN/Collections/interface/BNprimaryvertex.h"
#include "BEAN/Collections/interface/BNskimbits.h"
#include "BEAN/Collections/interface/BNsupercluster.h"
#include "BEAN/Collections/interface/BNtrack.h"
#include "BEAN/Collections/interface/BNtrigger.h"
#include "BEAN/Collections/interface/BNtrigobj.h"
#include "BEAN/Collections/interface/BNtau.h"
#include "BEAN/Collections/interface/BNgenjet.h"

#include "OSUT3Analysis/AnaTools/interface/AnalysisTypes.h"
#include "OSUT3Analysis/AnaTools/interface/BNstop.h"

template class edm::Handle<double>;
template class edm::Handle<std::vector<BNbxlumi, std::allocator<BNbxlumi> > >;
template class edm::Handle<std::vector<BNelectron, std::allocator<BNelectron> > >;
template class edm::Handle<std::vector<BNevent, std::allocator<BNevent> > >;
template class edm::Handle<std::vector<BNgenjet, std::allocator<BNgenjet> > >;
template class edm::Handle<std::vector<BNjet, std::allocator<BNjet> > >;
template class edm::Handle<std::vector<BNmcparticle, std::allocator<BNmcparticle> > >;
template class edm::Handle<std::vector<BNmet, std::allocator<BNmet> > >;
template class edm::Handle<std::vector<BNmuon, std::allocator<BNmuon> > >;
template class edm::Handle<std::vector<BNphoton, std::allocator<BNphoton> > >;
template class edm::Handle<std::vector<BNprimaryvertex, std::allocator<BNprimaryvertex> > >;
template class edm::Handle<std::vector<BNstop, std::allocator<BNstop> > >;
template class edm::Handle<std::vector<BNsupercluster, std::allocator<BNsupercluster> > >;
template class edm::Handle<std::vector<BNtau, std::allocator<BNtau> > >;
template class edm::Handle<std::vector<BNtrack, std::allocator<BNtrack> > >;
template class edm::Handle<std::vector<BNtrigger, std::allocator<BNtrigger> > >;
template class edm::Handle<std::vector<BNtrigobj, std::allocator<BNtrigobj> > >;
template class edm::OrphanHandle<std::map<std::string, std::vector<std::vector<bool, std::allocator<bool> >, std::allocator<std::vector<bool, std::allocator<bool> > > >, std::less<std::string>, std::allocator<std::pair<std::string const, std::vector<std::vector<bool, std::allocator<bool> >, std::allocator<std::vector<bool, std::allocator<bool> > > > > > > >;
template class edm::Wrapper<std::map<std::string, std::vector<std::vector<bool, std::allocator<bool> >, std::allocator<std::vector<bool, std::allocator<bool> > > >, std::less<std::string>, std::allocator<std::pair<std::string const, std::vector<std::vector<bool, std::allocator<bool> >, std::allocator<std::vector<bool, std::allocator<bool> > > > > > > >;
template class std::auto_ptr<std::map<std::string, std::vector<std::vector<bool, std::allocator<bool> >, std::allocator<std::vector<bool, std::allocator<bool> > > >, std::less<std::string>, std::allocator<std::pair<std::string const, std::vector<std::vector<bool, std::allocator<bool> >, std::allocator<std::vector<bool, std::allocator<bool> > > > > > > >;
template class std::istream_iterator<std::string, char, std::char_traits<char>, long>;
template class std::map<std::string, std::vector<std::vector<bool, std::allocator<bool> >, std::allocator<std::vector<bool, std::allocator<bool> > > >, std::less<std::string>, std::allocator<std::pair<std::string const, std::vector<std::vector<bool, std::allocator<bool> >, std::allocator<std::vector<bool, std::allocator<bool> > > > > > >;
template class std::pair<std::string, std::vector<std::vector<bool, std::allocator<bool> >, std::allocator<std::vector<bool, std::allocator<bool> > > > >;
template class std::vector<BadCSC, std::allocator<BadCSC> >;
template class std::vector<DeadEcal, std::allocator<DeadEcal> >;
template class std::vector<bool, std::allocator<bool> >;
template class std::vector<cut, std::allocator<cut> >;
template class std::vector<double, std::allocator<double> >;
template class std::vector<edm::ParameterSet, std::allocator<edm::ParameterSet> >;
template class std::vector<std::pair<edm::WrapperOwningHolder, edm::ConstBranchDescription const*>, std::allocator<std::pair<edm::WrapperOwningHolder, edm::ConstBranchDescription const*> > >;
template class std::vector<std::string, std::allocator<std::string> >;
template class std::vector<std::vector<bool, std::allocator<bool> >, std::allocator<std::vector<bool, std::allocator<bool> > > >;

template BadCSC* std::copy<BadCSC*, BadCSC*>(BadCSC*, BadCSC*, BadCSC*);
template DeadEcal* std::copy<DeadEcal*, DeadEcal*>(DeadEcal*, DeadEcal*, DeadEcal*);
template bool edm::Event::getByLabel<std::vector<BNbxlumi, std::allocator<BNbxlumi> > >(edm::InputTag const&, edm::Handle<std::vector<BNbxlumi, std::allocator<BNbxlumi> > >&) const;
template bool edm::Event::getByLabel<std::vector<BNelectron, std::allocator<BNelectron> > >(edm::InputTag const&, edm::Handle<std::vector<BNelectron, std::allocator<BNelectron> > >&) const;
template bool edm::Event::getByLabel<std::vector<BNevent, std::allocator<BNevent> > >(edm::InputTag const&, edm::Handle<std::vector<BNevent, std::allocator<BNevent> > >&) const;
template bool edm::Event::getByLabel<std::vector<BNgenjet, std::allocator<BNgenjet> > >(edm::InputTag const&, edm::Handle<std::vector<BNgenjet, std::allocator<BNgenjet> > >&) const;
template bool edm::Event::getByLabel<std::vector<BNjet, std::allocator<BNjet> > >(edm::InputTag const&, edm::Handle<std::vector<BNjet, std::allocator<BNjet> > >&) const;
template bool edm::Event::getByLabel<std::vector<BNmcparticle, std::allocator<BNmcparticle> > >(edm::InputTag const&, edm::Handle<std::vector<BNmcparticle, std::allocator<BNmcparticle> > >&) const;
template bool edm::Event::getByLabel<std::vector<BNmet, std::allocator<BNmet> > >(edm::InputTag const&, edm::Handle<std::vector<BNmet, std::allocator<BNmet> > >&) const;
template bool edm::Event::getByLabel<std::vector<BNmuon, std::allocator<BNmuon> > >(edm::InputTag const&, edm::Handle<std::vector<BNmuon, std::allocator<BNmuon> > >&) const;
template bool edm::Event::getByLabel<std::vector<BNphoton, std::allocator<BNphoton> > >(edm::InputTag const&, edm::Handle<std::vector<BNphoton, std::allocator<BNphoton> > >&) const;
template bool edm::Event::getByLabel<std::vector<BNprimaryvertex, std::allocator<BNprimaryvertex> > >(edm::InputTag const&, edm::Handle<std::vector<BNprimaryvertex, std::allocator<BNprimaryvertex> > >&) const;
template bool edm::Event::getByLabel<std::vector<BNsupercluster, std::allocator<BNsupercluster> > >(edm::InputTag const&, edm::Handle<std::vector<BNsupercluster, std::allocator<BNsupercluster> > >&) const;
template bool edm::Event::getByLabel<std::vector<BNtau, std::allocator<BNtau> > >(edm::InputTag const&, edm::Handle<std::vector<BNtau, std::allocator<BNtau> > >&) const;
template bool edm::Event::getByLabel<std::vector<BNtrack, std::allocator<BNtrack> > >(edm::InputTag const&, edm::Handle<std::vector<BNtrack, std::allocator<BNtrack> > >&) const;
template bool edm::Event::getByLabel<std::vector<BNtrigger, std::allocator<BNtrigger> > >(edm::InputTag const&, edm::Handle<std::vector<BNtrigger, std::allocator<BNtrigger> > >&) const;
template bool edm::Event::getByLabel<std::vector<BNtrigobj, std::allocator<BNtrigobj> > >(edm::InputTag const&, edm::Handle<std::vector<BNtrigobj, std::allocator<BNtrigobj> > >&) const;
template bool std::operator!=<char, std::char_traits<char>, std::allocator<char> >(std::basic_string<char, std::char_traits<char>, std::allocator<char> > const&, std::basic_string<char, std::char_traits<char>, std::allocator<char> > const&);
template bool std::operator< <char, std::char_traits<char>, std::allocator<char> >(std::basic_string<char, std::char_traits<char>, std::allocator<char> > const&, std::basic_string<char, std::char_traits<char>, std::allocator<char> > const&);
template bool std::operator<=<char, std::char_traits<char>, std::allocator<char> >(std::basic_string<char, std::char_traits<char>, std::allocator<char> > const&, std::basic_string<char, std::char_traits<char>, std::allocator<char> > const&);
template bool std::operator==<char, std::char_traits<char>, std::allocator<char> >(std::basic_string<char, std::char_traits<char>, std::allocator<char> > const&, char const*);
template bool std::operator><char, std::char_traits<char>, std::allocator<char> >(std::basic_string<char, std::char_traits<char>, std::allocator<char> > const&, std::basic_string<char, std::char_traits<char>, std::allocator<char> > const&);
template bool std::operator>=<char, std::char_traits<char>, std::allocator<char> >(std::basic_string<char, std::char_traits<char>, std::allocator<char> > const&, std::basic_string<char, std::char_traits<char>, std::allocator<char> > const&);
template boost::exception_ptr boost::exception_detail::get_static_exception_object<boost::exception_detail::bad_alloc_>();
template boost::exception_ptr boost::exception_detail::get_static_exception_object<boost::exception_detail::bad_exception_>();
template edm::OrphanHandle<std::map<std::string, std::vector<std::vector<bool, std::allocator<bool> >, std::allocator<std::vector<bool, std::allocator<bool> > > >, std::less<std::string>, std::allocator<std::pair<std::string const, std::vector<std::vector<bool, std::allocator<bool> >, std::allocator<std::vector<bool, std::allocator<bool> > > > > > > > edm::Event::put<std::map<std::string, std::vector<std::vector<bool, std::allocator<bool> >, std::allocator<std::vector<bool, std::allocator<bool> > > >, std::less<std::string>, std::allocator<std::pair<std::string const, std::vector<std::vector<bool, std::allocator<bool> >, std::allocator<std::vector<bool, std::allocator<bool> > > > > > > >(std::auto_ptr<std::map<std::string, std::vector<std::vector<bool, std::allocator<bool> >, std::allocator<std::vector<bool, std::allocator<bool> > > >, std::less<std::string>, std::allocator<std::pair<std::string const, std::vector<std::vector<bool, std::allocator<bool> >, std::allocator<std::vector<bool, std::allocator<bool> > > > > > > >, std::string const&);
template std::basic_string<char, std::char_traits<char>, std::allocator<char> > std::operator+<char, std::char_traits<char>, std::allocator<char> >(std::basic_string<char, std::char_traits<char>, std::allocator<char> >&&, char const*);
template std::string* std::copy<std::string*, std::string*>(std::string*, std::string*, std::string*);
template std::vector<bool, std::allocator<bool> >* std::__uninitialized_copy<false>::__uninit_copy<__gnu_cxx::__normal_iterator<std::vector<bool, std::allocator<bool> > const*, std::vector<std::vector<bool, std::allocator<bool> >, std::allocator<std::vector<bool, std::allocator<bool> > > > >, std::vector<bool, std::allocator<bool> >*>(__gnu_cxx::__normal_iterator<std::vector<bool, std::allocator<bool> > const*, std::vector<std::vector<bool, std::allocator<bool> >, std::allocator<std::vector<bool, std::allocator<bool> > > > >, __gnu_cxx::__normal_iterator<std::vector<bool, std::allocator<bool> > const*, std::vector<std::vector<bool, std::allocator<bool> >, std::allocator<std::vector<bool, std::allocator<bool> > > > >, std::vector<bool, std::allocator<bool> >*);
template std::vector<bool, std::allocator<bool> >* std::__uninitialized_copy<false>::__uninit_copy<std::move_iterator<std::vector<bool, std::allocator<bool> >*>, std::vector<bool, std::allocator<bool> >*>(std::move_iterator<std::vector<bool, std::allocator<bool> >*>, std::move_iterator<std::vector<bool, std::allocator<bool> >*>, std::vector<bool, std::allocator<bool> >*);
template std::vector<std::string, std::allocator<std::string> >::vector<std::istream_iterator<std::string, char, std::char_traits<char>, long> >(std::istream_iterator<std::string, char, std::char_traits<char>, long>, std::istream_iterator<std::string, char, std::char_traits<char>, long>, std::allocator<std::string> const&);
template void edm::convert_handle<std::vector<BNbxlumi, std::allocator<BNbxlumi> > >(edm::BasicHandle const&, edm::Handle<std::vector<BNbxlumi, std::allocator<BNbxlumi> > >&);
template void edm::convert_handle<std::vector<BNelectron, std::allocator<BNelectron> > >(edm::BasicHandle const&, edm::Handle<std::vector<BNelectron, std::allocator<BNelectron> > >&);
template void edm::convert_handle<std::vector<BNevent, std::allocator<BNevent> > >(edm::BasicHandle const&, edm::Handle<std::vector<BNevent, std::allocator<BNevent> > >&);
template void edm::convert_handle<std::vector<BNgenjet, std::allocator<BNgenjet> > >(edm::BasicHandle const&, edm::Handle<std::vector<BNgenjet, std::allocator<BNgenjet> > >&);
template void edm::convert_handle<std::vector<BNjet, std::allocator<BNjet> > >(edm::BasicHandle const&, edm::Handle<std::vector<BNjet, std::allocator<BNjet> > >&);
template void edm::convert_handle<std::vector<BNmcparticle, std::allocator<BNmcparticle> > >(edm::BasicHandle const&, edm::Handle<std::vector<BNmcparticle, std::allocator<BNmcparticle> > >&);
template void edm::convert_handle<std::vector<BNmet, std::allocator<BNmet> > >(edm::BasicHandle const&, edm::Handle<std::vector<BNmet, std::allocator<BNmet> > >&);
template void edm::convert_handle<std::vector<BNmuon, std::allocator<BNmuon> > >(edm::BasicHandle const&, edm::Handle<std::vector<BNmuon, std::allocator<BNmuon> > >&);
template void edm::convert_handle<std::vector<BNphoton, std::allocator<BNphoton> > >(edm::BasicHandle const&, edm::Handle<std::vector<BNphoton, std::allocator<BNphoton> > >&);
template void edm::convert_handle<std::vector<BNprimaryvertex, std::allocator<BNprimaryvertex> > >(edm::BasicHandle const&, edm::Handle<std::vector<BNprimaryvertex, std::allocator<BNprimaryvertex> > >&);
template void edm::convert_handle<std::vector<BNsupercluster, std::allocator<BNsupercluster> > >(edm::BasicHandle const&, edm::Handle<std::vector<BNsupercluster, std::allocator<BNsupercluster> > >&);
template void edm::convert_handle<std::vector<BNtau, std::allocator<BNtau> > >(edm::BasicHandle const&, edm::Handle<std::vector<BNtau, std::allocator<BNtau> > >&);
template void edm::convert_handle<std::vector<BNtrack, std::allocator<BNtrack> > >(edm::BasicHandle const&, edm::Handle<std::vector<BNtrack, std::allocator<BNtrack> > >&);
template void edm::convert_handle<std::vector<BNtrigger, std::allocator<BNtrigger> > >(edm::BasicHandle const&, edm::Handle<std::vector<BNtrigger, std::allocator<BNtrigger> > >&);
template void edm::convert_handle<std::vector<BNtrigobj, std::allocator<BNtrigobj> > >(edm::BasicHandle const&, edm::Handle<std::vector<BNtrigobj, std::allocator<BNtrigobj> > >&);
template void std::list<edm::ProductRegistryHelper::TypeLabelItem, std::allocator<edm::ProductRegistryHelper::TypeLabelItem> >::_M_insert<edm::BranchType, edm::TypeID const&, std::string const&>(std::_List_iterator<edm::ProductRegistryHelper::TypeLabelItem>, edm::BranchType&&, edm::TypeID const&, std::string const&);
template void std::list<edm::ProductRegistryHelper::TypeLabelItem, std::allocator<edm::ProductRegistryHelper::TypeLabelItem> >::emplace_back<edm::BranchType, edm::TypeID const&, std::string const&>(edm::BranchType&&, edm::TypeID const&, std::string const&);
template void std::vector<cut, std::allocator<cut> >::_M_emplace_back_aux<cut const&>(cut const&);
template void std::vector<double, std::allocator<double> >::_M_emplace_back_aux<double>(double&&);
template void std::vector<std::pair<edm::WrapperOwningHolder, edm::ConstBranchDescription const*>, std::allocator<std::pair<edm::WrapperOwningHolder, edm::ConstBranchDescription const*> > >::_M_emplace_back_aux<edm::WrapperOwningHolder const&, edm::ConstBranchDescription const*&>(edm::WrapperOwningHolder const&, edm::ConstBranchDescription const*&);
template void std::vector<std::pair<edm::WrapperOwningHolder, edm::ConstBranchDescription const*>, std::allocator<std::pair<edm::WrapperOwningHolder, edm::ConstBranchDescription const*> > >::emplace_back<edm::WrapperOwningHolder const&, edm::ConstBranchDescription const*&>(edm::WrapperOwningHolder const&, edm::ConstBranchDescription const*&);
template void std::vector<std::string, std::allocator<std::string> >::_M_emplace_back_aux<std::string const&>(std::string const&);
template void std::vector<std::string, std::allocator<std::string> >::_M_emplace_back_aux<std::string>(std::string&&);
template void std::vector<std::string, std::allocator<std::string> >::emplace_back<std::string>(std::string&&);
template void std::vector<std::vector<bool, std::allocator<bool> >, std::allocator<std::vector<bool, std::allocator<bool> > > >::_M_emplace_back_aux<std::vector<bool, std::allocator<bool> > >(std::vector<bool, std::allocator<bool> >&&);
