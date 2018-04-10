//
// Created by wang on 18-4-1.
//

#include "Motion.h"

namespace CMotion
{

    vector3d operator+(const vector3d& A, const vector3d& B)
    {
        vector3d C;
        C.x = A.x + B.x;
        C.y = A.y + B.y;
        C.z = A.z + B.z;
        return C;
    }

    vector3d operator-(const vector3d& A, const vector3d& B)
    {
        vector3d C;
        C.x = A.x - B.x;
        C.y = A.y - B.y;
        C.z = A.z - B.z;
        return C;
    }

    vector3d operator*(const vector3d& A, double dt)
    {
        vector3d B;
        B.x = A.x * dt;
        B.y = A.y * dt;
        B.z = A.z * dt;
        return B;
    }

    vector3d operator/(const vector3d& A, double dt)
    {
        vector3d B;
        B.x = A.x / dt;
        B.y = A.y / dt;
        B.z = A.z / dt;
        return B;
    }

    const status& Motion::getStatus() const
    {
        return _status.back();
    }

    const status& Motion::getStatus(double time) const
    {
        for (auto& i : _status)
        {
            if (i.time >= time)
            {
                return i;
            }
        }
        return _status.back();
    }
    const status& Motion::setAcceleration(status s, vector3d a)
    {
        auto& target = *(_status.end()-1);
        for (auto& i:_status)
        {
            if (i.time >= s.time)
            {
                target = i;
                break;
            }
        }
        target.acceleration = a;
        return target;
    }
    status Motion::move(const status& last)
    {
        status current;
        current.time = last.time + _dt;
        current.acceleration = {0.0, 0.0, 0.0};
        current.velocity = last.velocity + last.acceleration * _dt;
        current.position = last.position + last.velocity * _dt + last.acceleration * (0.5 * _dt * _dt);
        return current;
    }
    const status& Motion::updateMotion(double t)
    {
        auto start = _status.end()-1;
        for (auto i = _status.begin(); i != _status.end(); ++i)
        {
            if (i->time >= t)
            {
                start = i;
                break;
            }
        }
        for (auto last = start, curr = ++start; curr != _status.end(); ++curr,++last)
        {
            last->acceleration = curr->acceleration;
            *curr = move(*last);
        }
        return _status.back();
    }
    const status& Motion::appendMotion(vector3d last_a)
    {
        _status.back().acceleration = last_a;
        status sta_curr = move(_status.back());
        _status.push_back(sta_curr);
        return _status.back();
    }
    const status& Motion::appendMotion(const std::vector<vector3d>& a_list)
    {
        auto p_last = _status.end()-1;
        for (auto i : a_list)
        {
            p_last->acceleration = i;
            _status.insert(p_last+1, move(*p_last));
            ++p_last;
        }
        return this->updateMotion((p_last)->time);
    }
    const status& Motion::insertMotion(double time, const std::vector<vector3d>& a_list)
    {
        auto p_last = _status.end()-1;
        for (auto i = _status.begin(); i != _status.end(); ++i)
        {
            if (i->time >= time)
            {
                p_last = i;
                break;
            }
        }
        for (auto i : a_list)
        {
            p_last->acceleration = i;
            _status.insert(p_last+1, move(*p_last));
            ++p_last;
        }
        return this->updateMotion((p_last)->time);
    }
}// namespace CMotion