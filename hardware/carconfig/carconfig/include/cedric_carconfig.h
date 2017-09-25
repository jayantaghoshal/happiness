#ifndef CEDRIC_CARCONFIG_H
#define CEDRIC_CARCONFIG_H

#include <cstdint>

/*
 Usage:
   try
   {
     cedric::core::carconfig::v1::init(); // only once per application
   }
   catch(std::runtime_error const& error)
   {
     log_debug() << error.what();
   }

     std::uint8_t my_param = cedric::core::carconfig::getValue(101); // 101 is just an example

 Or

   #include <cedric_parameterlist.h>

   using namespace CarConfigParams;

   auto my_param = cedric::core::carconfig::getValue<CC1_VechicleTypeType>();
   if (my_param == CC1_VechicleTypeType::L541) ...

  Should you wish to use the older, deprecated, init function, then do:

  bool result = cedric::core::carconfig::v0::init(); // only once per application

*/

namespace cedric
{
namespace core
{
namespace carconfig
{

inline namespace v0
{
[[deprecated("Please switch to cedric::core::carconfig::v1::init")]]
int init();
}

namespace v1
{
/**
   Initialize the use of cedric carconfig
   Done once per application
   @exception throws std::runtimer_error if initialization fails
 */
void init();
}

/**
 * Get the value given the car config parameter number.
 * @param parameterNumber: according to car config specification. Example: 150
 * means Lane Keeping Aid (LKA), where 1->No LKA/LDW, 2->LKA, 5->LDW.
 * Return value is in the range 0x00..0xFF (0..255). Meaning is dependent on
 * the parameter number.
 * @exception throws std::out_of_range if parameterNumber is out of range
 * @exception throws std::runtime_error if init has not been called prior to this function
 */
std::uint8_t getValue(std::uint32_t parameterNumber);

/**
* Note: this method is only for providing raw CC values for MOST clients!
* Get the raw value given the car config parameter number.
* @param parameterNumber: according to car config specification. Example: 150
* means Lane Keeping Aid (LKA), where 1->No LKA/LDW, 2->LKA, 5->LDW.
* Return value is in the range 0x00..0xFF (0..255). Meaning is dependent on
* the parameter number.
* @exception throws std::out_of_range if parameterNumber is out of range
 * @exception throws std::runtime_error if init has not been called prior to this function
*/
std::uint8_t getRawValue(std::uint32_t parameterNumber);

/**
* Note: this method is only for providing CC status for MOST clients!
* @return true if CarConfig is in Configured state and false if in BULK state
*/
bool isConfigured();

/**
 * Templated version of getValue that can be used when types from
 * cedric_parameterlist.h
 * This templated function is provided as a convenience for users using cedric_parameterlist.h types
 * Parameter number is deduced from from appropriate enum class in cedric_parameterlist.h
 *        e.g. auto value = cedric::core::carconfig::getValue<CC1_VehicleTypeType>();
 * @return value (and type) from the same enum class as the parameterNumber came from
 * @exception throws std::out_of_range if parameterNumber is out of range
 * @exception throws std::runtime_error if init has not been called prior to this function
 */
template <typename T>
T getValue()
{
    return static_cast<T>(getValue(static_cast<std::uint32_t>(T::ParamNumber)));
}

/**
 * Template help method for checking a car config value
 * @param paramValue is an enum class value from cedric_parameterlist.h
 * e.g. if (cedric::core::carconfig::checkValue(CC1_VehicleTypeType::L541)) ...
 * @return true if the corresponding car config parameter has the value indicated by the input parameter.
 * @exception throws std::out_of_range if parameterNumber is out of range
 * @exception throws std::runtime_error if init has not been called prior to this function
 */
template <typename T>
bool checkValue(T paramValue)
{
    return (paramValue == getValue<T>());
}
}
}
}

#endif  // CEDRIC_CARCONFIG_H
