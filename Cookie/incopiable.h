#ifndef INCOPIABLE_H
#define INCOPIABLE_H


class Incopiable
{
    Incopiable(const Incopiable&) = delete;
    Incopiable& operator=(const Incopiable&) = delete;

protected:
    Incopiable() = default;
    ~Incopiable() = default;
};

#endif // INCOPIABLE_H
