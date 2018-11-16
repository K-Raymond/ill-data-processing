// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME detCal_rflx

/*******************************************************************/
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#define G__DICTIONARY
#include "RConfig.h"
#include "TClass.h"
#include "TDictAttributeMap.h"
#include "TInterpreter.h"
#include "TROOT.h"
#include "TBuffer.h"
#include "TMemberInspector.h"
#include "TInterpreter.h"
#include "TVirtualMutex.h"
#include "TError.h"

#ifndef G__ROOT
#define G__ROOT
#endif

#include "RtypesImp.h"
#include "TIsAProxy.h"
#include "TFileMergeInfo.h"
#include <algorithm>
#include "TCollectionProxyInfo.h"
/*******************************************************************/

#include "TDataMember.h"

// Since CINT ignores the std namespace, we need to do so in this file.
namespace std {} using namespace std;

// Header files passed as explicit arguments
#include "detCal.h"

// Header files passed via #pragma extra_include

namespace ROOT {
   static TClass *detCal_Dictionary();
   static void detCal_TClassManip(TClass*);
   static void *new_detCal(void *p = 0);
   static void *newArray_detCal(Long_t size, void *p);
   static void delete_detCal(void *p);
   static void deleteArray_detCal(void *p);
   static void destruct_detCal(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::detCal*)
   {
      ::detCal *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::detCal));
      static ::ROOT::TGenericClassInfo 
         instance("detCal", "", 22,
                  typeid(::detCal), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &detCal_Dictionary, isa_proxy, 0,
                  sizeof(::detCal) );
      instance.SetNew(&new_detCal);
      instance.SetNewArray(&newArray_detCal);
      instance.SetDelete(&delete_detCal);
      instance.SetDeleteArray(&deleteArray_detCal);
      instance.SetDestructor(&destruct_detCal);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::detCal*)
   {
      return GenerateInitInstanceLocal((::detCal*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::detCal*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *detCal_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::detCal*)0x0)->GetClass();
      detCal_TClassManip(theClass);
   return theClass;
   }

   static void detCal_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_detCal(void *p) {
      return  p ? new(p) ::detCal : new ::detCal;
   }
   static void *newArray_detCal(Long_t nElements, void *p) {
      return p ? new(p) ::detCal[nElements] : new ::detCal[nElements];
   }
   // Wrapper around operator delete
   static void delete_detCal(void *p) {
      delete ((::detCal*)p);
   }
   static void deleteArray_detCal(void *p) {
      delete [] ((::detCal*)p);
   }
   static void destruct_detCal(void *p) {
      typedef ::detCal current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::detCal

namespace {
  void TriggerDictionaryInitialization_detCal_rflx_Impl() {
    static const char* headers[] = {
0    };
    static const char* includePaths[] = {
"/home/kraymond/data_analysis/root/build/include",
"/home/kraymond/workspace/ill_120sn_data/macros/",
0
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "detCal_rflx dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_Autoloading_Map;
class detCal;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "detCal_rflx dictionary payload"

#ifndef G__VECTOR_HAS_CLASS_ITERATOR
  #define G__VECTOR_HAS_CLASS_ITERATOR 1
#endif

#define _BACKWARD_BACKWARD_WARNING_H
#include <string>
#include <sstream>
#include <vector>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <float.h>
#include <math.h>

// Enumerator for choosing which type of energy calibration
enum CalType {
    LINEAR,
    QUADRADIC
};

// Class to load in the experimental configuration
class detCal{
    // Initialize from a XPConfig
    public:

        /* Constructors and Initalizors */
        detCal();
        // Load experimenal configuration
        // XPConfig should be the path to XPConfig.txt
        detCal(std::string XPConfig);
        int loadCal(std::string XPConfig);
        void exportCal(std::string XPConfig);

        /* Energy calibration functions */
        double_t GetEnergy(double_t Q, int nDet, CalType Interpol = LINEAR );
        double_t GetCal( int nCoeff, int nDet );
        void SetCal( int nCoeff, int nDet , double_t Coeff);

        /* Functions for vetoing detector events */

        // For instance, if the detector is marked as
        // a BGO shield, these events should be ignored.
        bool isVito(int nDet);

        // Set the state of the detector
        void stateVitoDet(int nDet, bool state);

        // Return the number of channels loaded
        int NChan();

    private:
        // Calibration coefficents for
        // a0 + a1*Q = E
        
        std::vector<double_t> fCal0Vec;
        std::vector<double_t> fCal1Vec;
        std::vector<double_t> fCal2Vec;

        // Vito list
        std::vector<bool> fvitoVec; 

        // Detector type
        std::vector<int> fDetTypeVec;
};

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[]={
"detCal", payloadCode, "@",
nullptr};

    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("detCal_rflx",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_detCal_rflx_Impl, {}, classesHeaders);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_detCal_rflx_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_detCal_rflx() {
  TriggerDictionaryInitialization_detCal_rflx_Impl();
}
