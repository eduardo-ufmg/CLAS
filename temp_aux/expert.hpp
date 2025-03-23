// ... [Previous code unchanged until Expert section]

template <typename Derived>
class ExpertBase {
public:
  const ExpertID id;
  const Edge edge;
  const Coordinates midpoint;
  const NormalVector normal;
  const float bias;

  ExpertBase(const ExpertID id, const Edge edge);

private:
  Coordinates computeMidpoint(const Edge& edge) const;
  NormalVector computeNormal(const Edge& edge) const;
  float computeBias(const Coordinates& midpoint, 
                   const NormalVector& diff) const;
};

class ExpertV1 : public ExpertBase<ExpertV1> {
private:
  using ExpertBase::ExpertBase;

  // Implementation-specific methods
  NormalVector computeDifferencesImpl(const Edge& edge) const;
  float computeBiasImpl(const Coordinates& midpoint,
                       const NormalVector& diff) const;
};

class ExpertV2 : public ExpertBase<ExpertV2> {
private:
  using ExpertBase::ExpertBase;

  // Implementation-specific methods
  NormalVector computeDifferencesImpl(const Edge& edge) const;
  float computeBiasImpl(const Coordinates& midpoint,
                       const NormalVector& diff) const;
};

using ExpertsV1 = std::vector<ExpertV1>;
using ExpertsV2 = std::vector<ExpertV2>;

// ... [Remaining code unchanged]