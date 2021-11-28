#include "clutils/clutils.h"
#include <sstream>
#include <fmt/core.h>
#include <fmt/printf.h>
#include <fstream>

std::string get_build_log(const cl::BuildError &e)
{
    auto build_logs = e.getBuildLog();
    std::stringstream ss;
    ss << fmt::sprintf("%s: ", e.what());
    for (auto &&bl : build_logs)
    {
        ss << fmt::sprintf("\n\t%s: %s", bl.first.getInfo<CL_DEVICE_NAME>(), bl.second);
    }
    return ss.str();
}

std::array<cl_uint, 8> hex_to_sha256(
    const std::string &string)
{
    std::array<cl_uint, 8> bytes{};
    if (string.empty())
    {
        return bytes;
    }
    constexpr size_t isz = sizeof(cl_uint) * 2;
    for (size_t i = 0; i < 8; i++)
    {
        const std::string hex = string.substr(i * 8, i * 8 + 8);
        const cl_uint decimal = std::strtol(hex.c_str(), 0, 16);
        bytes[i] = decimal;
    }
    return bytes;
}

std::tuple<cl::NDRange, cl::NDRange, cl::NDRange> tuple_ranges_factory(const size_t max_dims)
{
    std::tuple<cl::NDRange, cl::NDRange, cl::NDRange> v{};
    switch (max_dims)
    {
    case 1:
        v = std::move(std::make_tuple<cl::NDRange, cl::NDRange, cl::NDRange>({0}, {0}, {0}));
        break;
    case 2:
        v = std::move(std::make_tuple<cl::NDRange, cl::NDRange, cl::NDRange>({0, 0}, {0, 0}, {0, 0}));
        break;
    case 3:
        v = std::move(std::make_tuple<cl::NDRange, cl::NDRange, cl::NDRange>({0, 0, 0}, {0, 0, 0}, {0, 0, 0}));
        break;
    default:
        v = std::move(std::make_tuple<cl::NDRange, cl::NDRange, cl::NDRange>({0}, {0}, {0}));
        break;
    }
    return v;
}

std::tuple<cl::NDRange, cl::NDRange, cl::NDRange> init_ranges(
    const size_t max_grp_size,
    const size_t multiple,
    const size_t max_dims)
{
    auto v = tuple_ranges_factory(max_dims);
    for (size_t i = 0; i < max_dims; i++)
    {
        std::get<1>(v).get()[i] = max_grp_size;
        std::get<2>(v).get()[i] = max_grp_size / multiple;
    }
    return v;
}

std::string read_file(const std::string &file_path)
{
    std::ifstream program_file(file_path.c_str());
    std::string program_string(std::istreambuf_iterator<char>(program_file),
                               (std::istreambuf_iterator<char>()));
    return program_string;
}

std::vector<cl::Platform> get_platforms()
{
    cl::vector<cl::Platform> platforms;
    cl::Platform::get(&platforms);
    return std::move(platforms);
}