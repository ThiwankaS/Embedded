#ifndef QUERIES_H
#define QUERIES_H

namespace hsl {

    // The Stop ID we want to monitor
    constexpr char kDefaultStopId[] = "HSL:1362151";

    // Part 1 of the query (everything before the ID)
    constexpr char kQueryPrefix[] = " { stop(id: \"";
    
    // Part 2 of the query (everything after the ID)
    constexpr char kQuerySuffix[] = "\") { name stoptimesWithoutPatterns(numberOfDepartures: 5) { "
                                    "realtimeArrival serviceDay scheduledArrival "
                                    "trip { routeShortName } } } }";

} // namespace hsl

#endif
