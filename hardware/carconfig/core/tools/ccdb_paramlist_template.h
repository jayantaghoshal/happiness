#ifndef CEDRIC_CCDB_PARAMETER_LIST_H
#define CEDRIC_CCDB_PARAMETER_LIST_H

namespace CarConfigParamList
{
    /* Do not edit by hand!
    The values below is extracted from the file ##FILENAME##
    on ##DATE## with the tool ##TOOL##
    */

    const std::map<int,std::vector<int>> paramList =
    {
        ##PARAMTEMPLATESTART##
        {
            ##PARAMNUMBER##, //##PARAMNAME##
            {
                0x##PARAMVALUENUMBER##, //##PARAMVALUE##
            }
        }##COMMASEPARATOR##
        ##PARAMTEMPLATEEND##
    };
}
#endif
