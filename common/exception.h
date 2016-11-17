//
// Created by tripack on 16-10-12.
//

#ifndef VE281_FA16_EXCEPTION_H
#define VE281_FA16_EXCEPTION_H

class Exception {};

class ExceptionDataset :    public Exception {};
class ExceptionGenerator :  public Exception {};

class ExceptionDatasetUninitialized :   public ExceptionDataset {};
class ExceptionOutOfRange :             public ExceptionDataset {};

class ExceptionGeneratorEmpty : public ExceptionGenerator {};

class ExceptionTruthTable : public Exception {};

class ExceptionInvalidVectorAssignment : public ExceptionTruthTable {};
class ExceptionTruthTableInvalidSize : public ExceptionTruthTable {};

#endif //VE281_FA16_EXCEPTION_H
