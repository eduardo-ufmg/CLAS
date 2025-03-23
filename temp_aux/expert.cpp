// ... [Existing implementations]

// ================== ExpertBase Implementation ==================
template <typename Derived>
ExpertBase<Derived>::ExpertBase(const ExpertID id, const Edge edge)
  : id(id), edge(edge),
    midpoint(computeMidpoint(edge)),
    normal(computeNormal(edge)),
    bias(computeBias(midpoint, normal)) {}

template <typename Derived>
Coordinates ExpertBase<Derived>::computeMidpoint(const Edge& edge) const {
  const auto& [v1, v2] = edge;
  const auto& c1 = v1->coordinates;
  const auto& c2 = v2->coordinates;

  Coordinates midpoint(c1.size());
  std::transform(c1.begin(), c1.end(), c2.begin(), midpoint.begin(),
                [](float x, float y) { return (x + y) / 2.0f; });
  return midpoint;
}

template <typename Derived>
NormalVector ExpertBase<Derived>::computeNormal(const Edge& edge) const {
  return static_cast<const Derived*>(this)->computeDifferencesImpl(edge);
}

template <typename Derived>
float ExpertBase<Derived>::computeBias(const Coordinates& midpoint,
                                      const NormalVector& diff) const {
  return static_cast<const Derived*>(this)->computeBiasImpl(midpoint, diff);
}

// ================== ExpertV1 Implementation ==================
NormalVector ExpertV1::computeDifferencesImpl(const Edge& edge) const {
  const auto& [v1, v2] = edge;
  const auto& c1 = v1->coordinates;
  const auto& c2 = v2->coordinates;

  NormalVector diff(c1.size());
  std::transform(c1.begin(), c1.end(), c2.begin(), diff.begin(),
                std::minus<float>());
  return diff;
}

float ExpertV1::computeBiasImpl(const Coordinates& midpoint,
                               const NormalVector& diff) const {
  return std::inner_product(midpoint.begin(), midpoint.end(),
                           diff.begin(), 0.0f);
}

// ================== ExpertV2 Implementation ==================
NormalVector ExpertV2::computeDifferencesImpl(const Edge& edge) const {
  const auto& [v1, v2] = edge;
  const auto& c1 = v1->coordinates;
  const auto& c2 = v2->coordinates;

  NormalVector diff(c1.size());
  std::transform(c2.begin(), c2.end(), c1.begin(), diff.begin(),
                std::minus<float>());
  return diff;
}

float ExpertV2::computeBiasImpl(const Coordinates& midpoint,
                               const NormalVector& diff) const {
  float sum = 0.0f;
  for (size_t i = 0; i < midpoint.size(); ++i) {
    sum += midpoint[i] * diff[i];
  }
  return sum;
}

// Explicit template instantiation
template class ExpertBase<ExpertV1>;
template class ExpertBase<ExpertV2>;