// ridefinizione operatore << per scrivere su uscite..
template <class T>
// inline Print& operator<<(Print& obj, T arg) {
Print& operator<<(Print& obj, T arg) {
    obj.print(arg);
    return obj;
    }

