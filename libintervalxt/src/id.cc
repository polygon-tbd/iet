/**********************************************************************
 *  This file is part of intervalxt.
 *
 *        Copyright (C) 2019 Vincent Delecroix
 *        Copyright (C) 2019 Julian Rüth
 *
 *  intervalxt is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  intervalxt is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with intervalxt. If not, see <https://www.gnu.org/licenses/>.
 *********************************************************************/

#include <boost/uuid/random_generator.hpp>

#include "intervalxt/detail/id.hpp"

#include "external/unique-factory/unique_factory.hpp"

namespace intervalxt {
namespace detail {

namespace {
auto& generator() {
  static boost::uuids::random_generator generator;
  return generator;
}

auto& id2uuid() {
  static UniqueFactory<boost::uuids::uuid, std::weak_ptr<const Id>> factory;
  return factory;
}

auto& uuid2id() {
  static UniqueFactory<const Id, boost::uuids::uuid> factory;
  return factory;
}
}

Id::Id() noexcept {}

Id::operator boost::uuids::uuid() const noexcept {
  // TODO: Can the factory handle a weak_ptr as the key?
  auto ret = *id2uuid().get(this->weak_from_this(), [&]() { 
    // TODO: Comment on why this is thread-safe.
    return new boost::uuids::uuid(generator()());
    // TODO: Is this still thread-safe if this method is called twice at the
    // same time?
  });
  uuid2id().get(ret, [&]()->std::shared_ptr<const Id> {
    // TODO: Comment on why this is thread-safe.
    return this->shared_from_this();
  });
  return ret;
}

std::shared_ptr<const Id> Id::make() noexcept {
  return std::shared_ptr<Id>(new Id());
}

std::shared_ptr<const Id> Id::make(const boost::uuids::uuid& uuid) noexcept {
  auto id = uuid2id().get(uuid, [&]() {
    return new Id();
  });
  id2uuid().get(id->weak_from_this(), [&]() {
    return new boost::uuids::uuid(uuid);
  });
  return id;
}
}
}
