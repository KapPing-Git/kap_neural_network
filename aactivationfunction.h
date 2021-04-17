#ifndef AACTIVATIONFUNCTION_H
#define AACTIVATIONFUNCTION_H

class AAbstructActivationFunction
{
public:
  virtual double forward(double x) = 0;
  virtual double backward(double x) = 0;
};

class A_none_function : public AAbstructActivationFunction
{
public:
  A_none_function() = default;

  // AAbstructActivationFunction interface
private:
  virtual double forward(double x) override {return (x>0)*x;}
  virtual double backward(double ) override {return 1;}
};

class A_ReLU_function : public AAbstructActivationFunction
{
public:
  A_ReLU_function() = default;

  // AAbstructActivationFunction interface
private:
  virtual double forward(double x) override {return (x>0)*x;}
  virtual double backward(double x) override {return (x>0);}
};

#endif // AACTIVATIONFUNCTION_H
