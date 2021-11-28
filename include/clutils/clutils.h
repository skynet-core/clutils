#ifndef CLUTILS_H
#define CLUTILS_H

#ifndef NO_STD
#ifndef CL_HPP_MINIMUM_OPENCL_VERSION
#define CL_HPP_MINIMUM_OPENCL_VERSION 120
#endif

#ifndef CL_HPP_TARGET_OPENCL_VERSION
#define CL_HPP_TARGET_OPENCL_VERSION 120
#endif

#ifndef CL_HPP_ENABLE_EXCEPTIONS
#define CL_HPP_ENABLE_EXCEPTIONS 1
#endif

#ifndef CL_HPP_ENABLE_PROGRAM_CONSTRUCTION_FROM_ARRAY_COMPATIBILITY
#define CL_HPP_ENABLE_PROGRAM_CONSTRUCTION_FROM_ARRAY_COMPATIBILITY 1
#endif
#endif

#ifdef WIN32
#define DllExport __declspec(dllexport)
#else
#define DllExport
#endif

#include <CL/opencl.hpp>

#include <string>
#include <tuple>
#include <array>

DllExport std::string get_build_log(const cl::BuildError &e);

DllExport std::array<cl_uint, 8> hex_to_sha256(const std::string &string);

DllExport std::tuple<cl::NDRange, cl::NDRange, cl::NDRange> tuple_ranges_factory(const size_t max_dims);

DllExport std::string read_file(const std::string &file_path);

DllExport std::tuple<cl::NDRange, cl::NDRange, cl::NDRange> init_ranges(
    const size_t max_grp_size,
    const size_t multiple,
    const size_t max_dims = 3);

DllExport std::vector<cl::Platform> get_platforms();
#endif