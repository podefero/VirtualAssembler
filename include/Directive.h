template <typename T> class Directive {
public:
  virtual ~Directive() = default;
  virtual unsigned int get_offset() = 0;
  virtual T get_value() = 0;
};
