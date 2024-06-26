﻿#pragma once
#include "ContactInfo.h"

struct CollisionResolution
{
public:
    static void ResolvePenetration(ContactInfo& contactInfo);
    static void ResolveCollision(ContactInfo& contactInfo);
};
