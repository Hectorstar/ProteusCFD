#ifndef SOLUTION_SPACE_H__
#define SOLUTION_SPACE_H__

#include "general.h"
#include "bc.h"
#include "dataInfo.h"
#include "solutionSpaceBase.h"
#include "timer.h"
#include "gradient.h"
#include "temporalControl.h"
#include "gaussian.h"
#include "postPlugin.h"
#include "exceptions.h"
#include "h5layer.h"
#include "move.h"

#include <string>
#include <vector>

//forward declarations
template <class Type> class CRS;
template <class Type> class Mesh;
template <class Type> class EqnSet;
template <class Type> class PObj;
template <class Type> class Param;
template <class Type> class Forces;
template <class Type> class Sensors;
template <class Type> class Limiter;
template <class Type> class TurbulenceModel;
template <class Type> class SolutionField;
template <class Type> class BoundaryConditions;
template <class Type> class GaussianSource;
template <class Type> class PostPlugin;

//storage class for solution variables, eqnset type and mesh associated with a 
//particular simulation region
template <class Type>
class SolutionSpace : public SolutionSpaceBase<Type>
{
public:
  template <class Type2>
  SolutionSpace(const SolutionSpace<Type2>& spaceToCopy);

  SolutionSpace(Param<Type>* param, PObj<Real>* p, std::string names, TemporalControl<Real>& temporalControl); 
  ~SolutionSpace();
  
  //all the internals of the class - forces, eqnset, etc. are built in here for modularity
  //this is essential b/c our copy constructor relies on it when copying from double to complex
  //and back again
  void Init();

  void AddField(DataInfo<Type> dataInfo, Int stateType, Int varLocation);
  void AddField(std::string name, Type refValue);
  void RemoveField(std::string name);
  SolutionField<Type> & GetField(std::string name);
  const SolutionField<Type> & GetField(std::string name) const;
  Type* GetFieldData(std::string name, Int state);
  //check for a fields existence
  Bool CheckField(std::string name) const;
  void WriteAvailableFields() const;

  //checks for all requested fields and aborts with error message if they are not present
  void ValidateRequestedFields() const;

  void InitCRSSystem();

  //work that must be done before timestep begins, only called once
  void PreIterate();
  //work that must be done once, before each timestep occurs
  void PreTimeAdvance();
  //work that must happen to advance to the next newton subiteration
  bool NewtonIterate();
  //work to advance in time, copy down solution vars, compute forces, etc.
  void PostTimeAdvance();

  //returns a pointer to a plugin with a given name
  PostPlugin<Type>* GetPlugin(std::string pluginName);
  
  //refreshes any internal data which may depend on a change in parameters
  void RefreshForParam();

  //clears old solution data from HDF file
  void ClearSolutionFromFile();
  void WriteSolution();

  void WriteRestartFile();
  void ReadRestartFile();

  void OpenOutFiles();
  void CloseOutFiles();

  void PrintTimers();

  Mesh<Type>* m;
  EqnSet<Type>* eqnset;
  EqnSet<RCmplx>* ceqnset;
  Param<Type> * param;
  Param<RCmplx>* cparam;
  BoundaryConditions<Real>* bc;
  Sensors<Type>* sensors;
  TurbulenceModel<Type>* turb;
  Forces<Type>* forces;
  Limiter<Type>* limiter;
  PObj<Type>* p;
  CRS<Type>* crs;
  Gradient<Type>* grad;
  GaussianSource<Type>* gaussian;
  
  //pointers for convenience
  Type* q;
  Type* qold;
  Type* qoldm1;
  Type* qgrad;


  std::ofstream timerOutFile;
  std::ofstream residOutFile;

private:


//residual data storage for CFL scaling
  Type residual;
  Type residualnm1;

  std::vector<SolutionField<Type>*> fields;
  std::vector<PostPlugin<Type>*> postPlugins;


  Bool isCopy;
  Type dtmin, residGlobal;
  Int nanflag;
};

//include implementations
#include "solutionSpace.tcc"

#endif
