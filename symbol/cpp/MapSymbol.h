#ifndef __MAPSYMBOL_H
#define __MAPSYMBOL_H

#include <map_engine_dll_exports.h>
#include <string>
#include <vector>
#include "SymShape.h"
#include "MapSymbol.h"
#include <json.h>

class DLL_EXPORT MapSymbol
{
public:
    MapSymbol();
    ~MapSymbol();


    bool fromJson(const std::string& jsontstr);
    bool fromJson(json_object* jsonobj);

    std::string toJson() const;

    std::string getErrorMessage() const;

protected:
    void clear();
protected:
    double _dotsPerMM;
    double _width, _height;

    std::vector<SymShape*> _shapes;

    std::string _errorMessage;
};

#endif