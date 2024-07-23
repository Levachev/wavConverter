#pragma once

#include "IConverter.h"

#include <map>

#include <memory>

class ICreator
{
 public:
  virtual ~ICreator( ) = default;

  virtual std::shared_ptr<IConverter> create() = 0;
};

template < typename T >
class Creator : public ICreator
{
 public:
  std::shared_ptr<IConverter> create()
  {
    return std::make_shared<T>();
  }
};

class Factory
{
 public:
  void register_creator(std::string converter_name, std::shared_ptr<ICreator> creator);
  std::shared_ptr<IConverter> create(const std::string &converter_name) const;
  bool can_create(std::string converter_name) const;

 private:
  std::map<std::string, std::shared_ptr<ICreator>> factory_map_;
};