#pragma once

namespace CarConfigParams
{
  /* Do not edit by hand!
     The values below is extracted from the file ##FILENAME##
     on ##DATE## with the tool ##TOOL##
   */

##PARAMTEMPLATESTART##
  /* Car config parameter number ##PARAMNUMBER## and defined values
      Parameter Description:
       ##PARAMDESC##
   */

  enum class CC##PARAMNUMBER##_##PARAMNAME##Type
  {
      ParamNumber = ##PARAMNUMBER##,
      ##PARAMVALUE## = 0x##PARAMVALUENUMBER##,
      InvalidValue = 0xFF
  };

##PARAMTEMPLATEEND##
}

