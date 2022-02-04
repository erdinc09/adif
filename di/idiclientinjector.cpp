/*
 * Copyright (c) 2020-2021 Ahmet Erdinç Yılmaz -- <ahmeterdinc09@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the  Apache License Version 2.0. Please see the LICENSE
 * file.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.
 */

#include "idiclientinjector.h"
namespace adif {
namespace internal {
IDIClientInjector::IDIClientInjector() { 
    DependencyProvider::dependencyProvider.addClient(this);
 }
}  // namespace internal
}  // namespace adif
