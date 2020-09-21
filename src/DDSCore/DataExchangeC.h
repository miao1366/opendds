// -*- C++ -*-
// $Id$

/**
 * Code generated by the The ACE ORB (TAO) IDL Compiler v2.2a_p12
 * TAO and the TAO IDL Compiler have been developed by:
 *       Center for Distributed Object Computing
 *       Washington University
 *       St. Louis, MO
 *       USA
 *       http://www.cs.wustl.edu/~schmidt/doc-center.html
 * and
 *       Distributed Object Computing Laboratory
 *       University of California at Irvine
 *       Irvine, CA
 *       USA
 * and
 *       Institute for Software Integrated Systems
 *       Vanderbilt University
 *       Nashville, TN
 *       USA
 *       http://www.isis.vanderbilt.edu/
 *
 * Information about TAO is available at:
 *     http://www.cs.wustl.edu/~schmidt/TAO.html
 **/

// TAO_IDL - Generated from
// e:\opendds-3.12\ace_wrappers\tao\tao_idl\be\be_codegen.cpp:152

#ifndef _TAO_IDL_DATAEXCHANGEC_FCIXAZ_H_
#define _TAO_IDL_DATAEXCHANGEC_FCIXAZ_H_

#include /**/ "ace/pre.h"


#include /**/ "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */


#include "tao/ORB.h"
#include "tao/Basic_Types.h"
#include "dds/DCPS/ZeroCopyInfoSeq_T.h"
#include "tao/String_Manager_T.h"
#include "tao/Sequence_T.h"
#include "tao/Seq_Var_T.h"
#include "tao/Seq_Out_T.h"
#include "tao/VarOut_T.h"
#include "tao/Arg_Traits_T.h"
#include "tao/Basic_Arguments.h"
#include "tao/Special_Basic_Arguments.h"
#include "tao/Any_Insert_Policy_T.h"
#include "tao/Fixed_Size_Argument_T.h"
#include "tao/Var_Size_Argument_T.h"
#include "tao/UB_String_Arguments.h"
#include /**/ "tao/Version.h"
#include /**/ "tao/Versioned_Namespace.h"

#include "dds/DdsDcpsCoreC.h"
#include "tao/CharSeqC.h"

#if TAO_MAJOR_VERSION != 2 || TAO_MINOR_VERSION != 2 || TAO_BETA_VERSION != 0
#error This file should be regenerated with TAO_IDL
#endif

#if defined (TAO_EXPORT_MACRO)
#undef TAO_EXPORT_MACRO
#endif
#define TAO_EXPORT_MACRO 

// TAO_IDL - Generated from
// e:\opendds-3.12\ace_wrappers\tao\tao_idl\be\be_visitor_module\module_ch.cpp:38

namespace Data_Exchange_Platform
{

  // TAO_IDL - Generated from
  // e:\opendds-3.12\ace_wrappers\tao\tao_idl\be\be_type.cpp:261

  struct ObjectAttribute;

  typedef
    ::TAO_Var_Var_T<
        ObjectAttribute
      >
    ObjectAttribute_var;

  typedef
    ::TAO_Out_T<
        ObjectAttribute
      >
    ObjectAttribute_out;

  // TAO_IDL - Generated from
  // e:\opendds-3.12\ace_wrappers\tao\tao_idl\be\be_visitor_structure\structure_ch.cpp:51

  struct  ObjectAttribute
  {

    // TAO_IDL - Generated from
    // e:\opendds-3.12\ace_wrappers\tao\tao_idl\be\be_type.cpp:307

    
    typedef ObjectAttribute_var _var_type;
    typedef ObjectAttribute_out _out_type;
    
    ::TAO::String_Manager instanceName;
    ::TAO::String_Manager className;
    ::TAO::String_Manager attributeName;
    ::TAO::String_Manager attributeType;
    CORBA::CharSeq attributeValue;
  };

  // TAO_IDL - Generated from
  // e:\opendds-3.12\ace_wrappers\tao\tao_idl\be\be_type.cpp:261

  struct InterationParameter;

  typedef
    ::TAO_Var_Var_T<
        InterationParameter
      >
    InterationParameter_var;

  typedef
    ::TAO_Out_T<
        InterationParameter
      >
    InterationParameter_out;

  // TAO_IDL - Generated from
  // e:\opendds-3.12\ace_wrappers\tao\tao_idl\be\be_visitor_structure\structure_ch.cpp:51

  struct  InterationParameter
  {

    // TAO_IDL - Generated from
    // e:\opendds-3.12\ace_wrappers\tao\tao_idl\be\be_type.cpp:307

    
    typedef InterationParameter_var _var_type;
    typedef InterationParameter_out _out_type;
    
    ::TAO::String_Manager parameterName;
    ::TAO::String_Manager parameterType;
    CORBA::CharSeq parameterValue;
  };

  // TAO_IDL - Generated from
  // e:\opendds-3.12\ace_wrappers\tao\tao_idl\be\be_visitor_sequence\sequence_ch.cpp:102

#if !defined (_DATA_EXCHANGE_PLATFORM_PARAMLST_CH_)
#define _DATA_EXCHANGE_PLATFORM_PARAMLST_CH_

  class paramlst;

  typedef
    ::TAO_VarSeq_Var_T<
        paramlst
      >
    paramlst_var;

  typedef
    ::TAO_Seq_Out_T<
        paramlst
      >
    paramlst_out;

  class  paramlst
    : public
        ::TAO::unbounded_value_sequence<
            InterationParameter
          >
  {
  public:
    paramlst (void);
    paramlst ( ::CORBA::ULong max);
    paramlst (
      ::CORBA::ULong max,
      ::CORBA::ULong length,
      InterationParameter* buffer,
      ::CORBA::Boolean release = false);
    paramlst (const paramlst &);
    virtual ~paramlst (void);
    

    // TAO_IDL - Generated from
    // e:\opendds-3.12\ace_wrappers\tao\tao_idl\be\be_type.cpp:307

    
    typedef paramlst_var _var_type;
    typedef paramlst_out _out_type;
  };

#endif /* end #if !defined */

  // TAO_IDL - Generated from
  // e:\opendds-3.12\ace_wrappers\tao\tao_idl\be\be_type.cpp:261

  struct Interation;

  typedef
    ::TAO_Var_Var_T<
        Interation
      >
    Interation_var;

  typedef
    ::TAO_Out_T<
        Interation
      >
    Interation_out;

  // TAO_IDL - Generated from
  // e:\opendds-3.12\ace_wrappers\tao\tao_idl\be\be_visitor_structure\structure_ch.cpp:51

  struct  Interation
  {

    // TAO_IDL - Generated from
    // e:\opendds-3.12\ace_wrappers\tao\tao_idl\be\be_type.cpp:307

    
    typedef Interation_var _var_type;
    typedef Interation_out _out_type;
    
    ::CORBA::LongLong timeStamp;
    ::TAO::String_Manager interationName;
    Data_Exchange_Platform::paramlst parameterList;
  };

  // TAO_IDL - Generated from
  // e:\opendds-3.12\ace_wrappers\tao\tao_idl\be\be_visitor_sequence\sequence_ch.cpp:102

#if !defined (_DATA_EXCHANGE_PLATFORM_INTERATIONLIST_CH_)
#define _DATA_EXCHANGE_PLATFORM_INTERATIONLIST_CH_

  class InterationList;

  typedef
    ::TAO_VarSeq_Var_T<
        InterationList
      >
    InterationList_var;

  typedef
    ::TAO_Seq_Out_T<
        InterationList
      >
    InterationList_out;

  class  InterationList
    : public
        ::TAO::unbounded_value_sequence<
            Interation
          >
  {
  public:
    InterationList (void);
    InterationList ( ::CORBA::ULong max);
    InterationList (
      ::CORBA::ULong max,
      ::CORBA::ULong length,
      Interation* buffer,
      ::CORBA::Boolean release = false);
    InterationList (const InterationList &);
    virtual ~InterationList (void);
    

    // TAO_IDL - Generated from
    // e:\opendds-3.12\ace_wrappers\tao\tao_idl\be\be_type.cpp:307

    
    typedef InterationList_var _var_type;
    typedef InterationList_out _out_type;
  };

#endif /* end #if !defined */

  // TAO_IDL - Generated from
  // e:\opendds-3.12\ace_wrappers\tao\tao_idl\be\be_type.cpp:261

  struct Interations;

  typedef
    ::TAO_Var_Var_T<
        Interations
      >
    Interations_var;

  typedef
    ::TAO_Out_T<
        Interations
      >
    Interations_out;

  // TAO_IDL - Generated from
  // e:\opendds-3.12\ace_wrappers\tao\tao_idl\be\be_visitor_structure\structure_ch.cpp:51

  struct  Interations
  {

    // TAO_IDL - Generated from
    // e:\opendds-3.12\ace_wrappers\tao\tao_idl\be\be_type.cpp:307

    
    typedef Interations_var _var_type;
    typedef Interations_out _out_type;
    
    ::CORBA::LongLong timeStamp;
    Data_Exchange_Platform::InterationList interationlst;
  };

// TAO_IDL - Generated from
// e:\opendds-3.12\ace_wrappers\tao\tao_idl\be\be_visitor_module\module_ch.cpp:67

} // module Data_Exchange_Platform

// TAO_IDL - Generated from
// e:\opendds-3.12\ace_wrappers\tao\tao_idl\be\be_visitor_arg_traits.cpp:68

TAO_BEGIN_VERSIONED_NAMESPACE_DECL


// Arg traits specializations.
namespace TAO
{

  // TAO_IDL - Generated from
  // e:\opendds-3.12\ace_wrappers\tao\tao_idl\be\be_visitor_arg_traits.cpp:947

  template<>
  class Arg_Traits< ::Data_Exchange_Platform::ObjectAttribute>
    : public
        Var_Size_Arg_Traits_T<
            ::Data_Exchange_Platform::ObjectAttribute,
            TAO::Any_Insert_Policy_Noop
          >
  {
  };

  // TAO_IDL - Generated from
  // e:\opendds-3.12\ace_wrappers\tao\tao_idl\be\be_visitor_arg_traits.cpp:947

  template<>
  class Arg_Traits< ::Data_Exchange_Platform::InterationParameter>
    : public
        Var_Size_Arg_Traits_T<
            ::Data_Exchange_Platform::InterationParameter,
            TAO::Any_Insert_Policy_Noop
          >
  {
  };

  // TAO_IDL - Generated from
  // e:\opendds-3.12\ace_wrappers\tao\tao_idl\be\be_visitor_arg_traits.cpp:685

  template<>
  class Arg_Traits< ::Data_Exchange_Platform::paramlst>
    : public
        Var_Size_Arg_Traits_T<
            ::Data_Exchange_Platform::paramlst,
            TAO::Any_Insert_Policy_Noop
          >
  {
  };

  // TAO_IDL - Generated from
  // e:\opendds-3.12\ace_wrappers\tao\tao_idl\be\be_visitor_arg_traits.cpp:947

  template<>
  class Arg_Traits< ::Data_Exchange_Platform::Interation>
    : public
        Var_Size_Arg_Traits_T<
            ::Data_Exchange_Platform::Interation,
            TAO::Any_Insert_Policy_Noop
          >
  {
  };

  // TAO_IDL - Generated from
  // e:\opendds-3.12\ace_wrappers\tao\tao_idl\be\be_visitor_arg_traits.cpp:685

  template<>
  class Arg_Traits< ::Data_Exchange_Platform::InterationList>
    : public
        Var_Size_Arg_Traits_T<
            ::Data_Exchange_Platform::InterationList,
            TAO::Any_Insert_Policy_Noop
          >
  {
  };

  // TAO_IDL - Generated from
  // e:\opendds-3.12\ace_wrappers\tao\tao_idl\be\be_visitor_arg_traits.cpp:947

  template<>
  class Arg_Traits< ::Data_Exchange_Platform::Interations>
    : public
        Var_Size_Arg_Traits_T<
            ::Data_Exchange_Platform::Interations,
            TAO::Any_Insert_Policy_Noop
          >
  {
  };
}

TAO_END_VERSIONED_NAMESPACE_DECL



// TAO_IDL - Generated from
// e:\opendds-3.12\ace_wrappers\tao\tao_idl\be\be_visitor_traits.cpp:62

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

// Traits specializations.
namespace TAO
{
}
TAO_END_VERSIONED_NAMESPACE_DECL



// TAO_IDL - Generated from
// e:\opendds-3.12\ace_wrappers\tao\tao_idl\be\be_visitor_structure\cdr_op_ch.cpp:46


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

 ::CORBA::Boolean operator<< (TAO_OutputCDR &, const Data_Exchange_Platform::ObjectAttribute &);
 ::CORBA::Boolean operator>> (TAO_InputCDR &, Data_Exchange_Platform::ObjectAttribute &);

TAO_END_VERSIONED_NAMESPACE_DECL



// TAO_IDL - Generated from
// e:\opendds-3.12\ace_wrappers\tao\tao_idl\be\be_visitor_structure\cdr_op_ch.cpp:46


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

 ::CORBA::Boolean operator<< (TAO_OutputCDR &, const Data_Exchange_Platform::InterationParameter &);
 ::CORBA::Boolean operator>> (TAO_InputCDR &, Data_Exchange_Platform::InterationParameter &);

TAO_END_VERSIONED_NAMESPACE_DECL



// TAO_IDL - Generated from
// e:\opendds-3.12\ace_wrappers\tao\tao_idl\be\be_visitor_sequence\cdr_op_ch.cpp:68

#if !defined _TAO_CDR_OP_Data_Exchange_Platform_paramlst_H_
#define _TAO_CDR_OP_Data_Exchange_Platform_paramlst_H_
TAO_BEGIN_VERSIONED_NAMESPACE_DECL


 ::CORBA::Boolean operator<< (
    TAO_OutputCDR &strm,
    const Data_Exchange_Platform::paramlst &_tao_sequence);
 ::CORBA::Boolean operator>> (
    TAO_InputCDR &strm,
    Data_Exchange_Platform::paramlst &_tao_sequence);

TAO_END_VERSIONED_NAMESPACE_DECL



#endif /* _TAO_CDR_OP_Data_Exchange_Platform_paramlst_H_ */

// TAO_IDL - Generated from
// e:\opendds-3.12\ace_wrappers\tao\tao_idl\be\be_visitor_structure\cdr_op_ch.cpp:46


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

 ::CORBA::Boolean operator<< (TAO_OutputCDR &, const Data_Exchange_Platform::Interation &);
 ::CORBA::Boolean operator>> (TAO_InputCDR &, Data_Exchange_Platform::Interation &);

TAO_END_VERSIONED_NAMESPACE_DECL



// TAO_IDL - Generated from
// e:\opendds-3.12\ace_wrappers\tao\tao_idl\be\be_visitor_sequence\cdr_op_ch.cpp:68

#if !defined _TAO_CDR_OP_Data_Exchange_Platform_InterationList_H_
#define _TAO_CDR_OP_Data_Exchange_Platform_InterationList_H_
TAO_BEGIN_VERSIONED_NAMESPACE_DECL


 ::CORBA::Boolean operator<< (
    TAO_OutputCDR &strm,
    const Data_Exchange_Platform::InterationList &_tao_sequence);
 ::CORBA::Boolean operator>> (
    TAO_InputCDR &strm,
    Data_Exchange_Platform::InterationList &_tao_sequence);

TAO_END_VERSIONED_NAMESPACE_DECL



#endif /* _TAO_CDR_OP_Data_Exchange_Platform_InterationList_H_ */

// TAO_IDL - Generated from
// e:\opendds-3.12\ace_wrappers\tao\tao_idl\be\be_visitor_structure\cdr_op_ch.cpp:46


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

 ::CORBA::Boolean operator<< (TAO_OutputCDR &, const Data_Exchange_Platform::Interations &);
 ::CORBA::Boolean operator>> (TAO_InputCDR &, Data_Exchange_Platform::Interations &);

TAO_END_VERSIONED_NAMESPACE_DECL



// TAO_IDL - Generated from
// e:\opendds-3.12\ace_wrappers\tao\tao_idl\be\be_codegen.cpp:1703
#if defined (__ACE_INLINE__)
#include "DataExchangeC.inl"
#endif /* defined INLINE */

#include /**/ "ace/post.h"

#endif /* ifndef */
