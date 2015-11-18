#ifndef PENCILERROR_H
#define PENCILERROR_H

#include <QString>


enum ErrorCode
{
    PCL_OK = 0,
    PCL_FAIL,
    
    // for Object loading
    ERROR_FILE_NOT_EXIST,
    ERROR_FILE_CANNOT_OPEN,
    ERROR_INVALID_XML_FILE,
    ERROR_INVALID_PENCIL_FILE,

    //
    ERROR_INVALID_LAYER_TYPE,
    ERROR_LOAD_IMAGE_FAIL,
};


class Error
{
public:
    Error() { mCode = PCL_OK; }
    Error( ErrorCode eCode ) { mCode = eCode; }
    ErrorCode code() { return mCode; }

    QString msg();

private:
    ErrorCode mCode;
};

#endif // PENCILERROR_H
