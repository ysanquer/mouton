#ifndef _METRIC_H
#define _METRIC_H

///
/// \brief Defines an object that can evaluate its distance to another
///  "metric object"
///
/// \authors Yann Sanquer
/// \authors Jeremy Montanes
///
template<typename MetricObject, typename MetricValue>
class Metric
{
    public:
        ///
        /// \brief Destructor
        ///
        virtual ~Metric() = 0;

        ///
        /// \brief Evaluate distance to a metric object
        ///
        virtual MetricValue distance(MetricObject const &) const = 0;
};

template<typename MetricObject, typename MetricValue>
inline Metric<MetricObject, MetricValue>::~Metric()
{}

#endif 
