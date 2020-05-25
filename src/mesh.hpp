#pragma once

#include <boost/filesystem.hpp>
#include <string>
#include <vector>
#include <exception>
#include <iosfwd>
#include <cassert>

namespace aste {

namespace fs = boost::filesystem;
class BaseName;
struct Mesh;
class MeshName;

class MeshException : public std::runtime_error {
  public:
    MeshException(const std::string& what_arg):std::runtime_error(what_arg){};
};

struct ExecutionContext {
  ExecutionContext() = default;
  ExecutionContext(int rank, int size) : rank(rank), size(size) {
    assert(0 <= rank && rank < size);
  };
  int rank{0};
  int size{1};
  bool isParallel() const { return size > 1; }
};

class MeshName {
public:
  MeshName() = default;

  std::string filename() const;

  std::string connectivityfilename() const;

  Mesh load() const;

  void save(const Mesh& mesh) const;

private:

  MeshName(std::string meshname) : _mname(std::move(meshname)) {}

  std::string _mname;

  friend BaseName;
};

std::ostream& operator<<(std::ostream& out, const MeshName& mname);

class BaseName {
public:
  BaseName(std::string basename) : _bname(std::move(basename)) {}

  MeshName with(const ExecutionContext &context) const;

  std::vector<MeshName> findAll(const ExecutionContext &context) const;

private:
  std::string _bname;
};

struct Mesh {
  std::vector<std::array<double, 3>> positions;
  std::vector<std::array<size_t, 2>> edges;
  std::vector<std::array<size_t, 3>> triangles;
  std::vector<double> data;

  std::string previewData(std::size_t max = 10) const;
  std::string summary() const;
};

} // namespace aste
