#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct sRange
{
    unsigned int sourceStart;
    unsigned int destStart;
    unsigned int size;
} sRange;

typedef struct sMap
{
    sRange *ranges;
    size_t rangesLength;
} sMap;

typedef struct sMaps
{
    sMap seedToSoil;
    sMap soilToFertilizer;
    sMap fertilizerToWater;
    sMap waterToLight;
    sMap lightToTemperature;
    sMap temperatureToHumidity;
    sMap humidityToLocation;
} sMaps;

void fillRanges(sMap *map, const sRange *ranges, const size_t rangesLength)
{
    if (rangesLength == 0 || ranges == NULL || map == NULL)
    {
        return;
    }
    map->ranges = (sRange *)malloc(sizeof(sRange) * rangesLength);
    memcpy(map->ranges, ranges, sizeof(sRange) * rangesLength);
    map->rangesLength = rangesLength;
}

int parseInput(const char *inputFilename, unsigned long **outSeeds, size_t *seedsSize, sMaps *outMaps)
{
    FILE *fp;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    size_t MAX_SEEDS = 1024;
    unsigned long seeds[MAX_SEEDS];
    size_t iSeed = 0;

    fp = fopen(inputFilename, "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);

    // First the seeds
    if (getline(&line, &len, fp) != -1)
    {
        char *strToken = strtok(&line[sizeof("seeds: ") - 1], " ");
        while (strToken != NULL)
        {
            char *end;
            seeds[iSeed] = strtoul(strToken, &end, 10);
            if (end != strToken)
            {
                iSeed++;
            }
            strToken = strtok(NULL, " ");
        }
    }
    *outSeeds = (unsigned long *)malloc(iSeed * sizeof(unsigned long));
    memcpy(*outSeeds, seeds, iSeed * sizeof(unsigned long));
    *seedsSize = iSeed;

    sMap *filling = &outMaps->seedToSoil;
    sRange tmpRanges[256];
    size_t currentRange = 0;
    // Then the maps
    while (getline(&line, &len, fp) != -1)
    {
        printf("%d strcmp\n", strcmp(line, "humidity-to-location map:\n"));

        if (strcmp(line, "seed-to-soil map:\n") == 0)
        {
            fillRanges(filling, tmpRanges, currentRange);
            currentRange = 0;
            filling = &outMaps->seedToSoil;
        }
        else if (strcmp(line, "soil-to-fertilizer map:\n") == 0)
        {
            fillRanges(filling, tmpRanges, currentRange);
            currentRange = 0;
            filling = &outMaps->soilToFertilizer;
        }
        else if (strcmp(line, "fertilizer-to-water map:\n") == 0)
        {
            fillRanges(filling, tmpRanges, currentRange);
            currentRange = 0;
            filling = &outMaps->fertilizerToWater;
        }
        else if (strcmp(line, "water-to-light map:\n") == 0)
        {
            fillRanges(filling, tmpRanges, currentRange);
            currentRange = 0;
            filling = &outMaps->waterToLight;
        }
        else if (strcmp(line, "light-to-temperature map:\n") == 0)
        {
            fillRanges(filling, tmpRanges, currentRange);
            currentRange = 0;
            filling = &outMaps->lightToTemperature;
        }
        else if (strcmp(line, "temperature-to-humidity map:\n") == 0)
        {
            fillRanges(filling, tmpRanges, currentRange);
            currentRange = 0;
            filling = &outMaps->temperatureToHumidity;
        }
        else if (strcmp(line, "humidity-to-location map:\n") == 0)
        {
            printf("HUMIDITYYYY\n");
            fillRanges(filling, tmpRanges, currentRange);
            currentRange = 0;
            filling = &outMaps->humidityToLocation;
        }
        else
        {
            int read = sscanf(line, "%d %d %d", &tmpRanges[currentRange].destStart, &tmpRanges[currentRange].sourceStart, &tmpRanges[currentRange].size);
            // printf("read %d\n", read);
            if (read == 3)
            {
                currentRange++;
            }
        }
    }
    fclose(fp);
    fillRanges(filling, tmpRanges, currentRange);
}

unsigned int advanceInMap(const unsigned int source, const sMap *map)
{
    for (size_t i = 0; i < map->rangesLength; i++)
    {
        sRange r = map->ranges[i];
        if (source >= r.sourceStart && source < r.sourceStart + r.size)
        {
            unsigned int offset = source - r.sourceStart;
            return r.destStart + offset;
        }
    }
    return source;
}

int puzzle1(const unsigned long *seeds, const size_t seedsLength, const sMaps *maps)
{
    unsigned int *seedsToLocation = (unsigned int *)malloc(seedsLength * sizeof(unsigned long));

    for (size_t i = 0; i < seedsLength; i++)
    {
        seedsToLocation[i] = advanceInMap(seeds[i], &maps->seedToSoil);
    }
    for (size_t i = 0; i < seedsLength; i++)
    {
        seedsToLocation[i] = advanceInMap(seedsToLocation[i], &maps->soilToFertilizer);
    }
    for (size_t i = 0; i < seedsLength; i++)
    {
        seedsToLocation[i] = advanceInMap(seedsToLocation[i], &maps->fertilizerToWater);
    }
    for (size_t i = 0; i < seedsLength; i++)
    {
        seedsToLocation[i] = advanceInMap(seedsToLocation[i], &maps->waterToLight);
    }
    for (size_t i = 0; i < seedsLength; i++)
    {
        seedsToLocation[i] = advanceInMap(seedsToLocation[i], &maps->lightToTemperature);
    }
    for (size_t i = 0; i < seedsLength; i++)
    {
        seedsToLocation[i] = advanceInMap(seedsToLocation[i], &maps->temperatureToHumidity);
    }
    for (size_t i = 0; i < seedsLength; i++)
    {
        seedsToLocation[i] = advanceInMap(seedsToLocation[i], &maps->humidityToLocation);
    }
    unsigned long min = ULONG_MAX;
    for (size_t i = 0; i < seedsLength; i++)
    {
        printf("Seed: %u, location: %u\n", seeds[i], seedsToLocation[i]);
        min = seedsToLocation[i] < min ? seedsToLocation[i] : min;
    }
    if (seedsToLocation != NULL)
    {
        free(seedsToLocation);
        seedsToLocation = NULL;
    }
    return min;
}

typedef struct sSeed
{
    unsigned long begin;
    unsigned long size;
} sSeed;

typedef struct sLinkedSeed
{
    sSeed seed;
    struct sLinkedSeed *next;
} sLinkedSeed;

sLinkedSeed *createLinkedSeed(const sSeed *originalSeed)
{
    sLinkedSeed *linked = (sLinkedSeed *)malloc(sizeof(sLinkedSeed));
    memcpy(&linked->seed, originalSeed, sizeof(sSeed));
    linked->next = NULL;
    return linked;
}

void addSeed(sLinkedSeed *linkedSeed, const sSeed *seedToAdd)
{
    sLinkedSeed *cur = linkedSeed;
    while (cur->next != NULL)
    {
        cur = cur->next;
    }
    cur->next = (sLinkedSeed *)malloc(sizeof(sLinkedSeed));
    memcpy(&cur->next->seed, seedToAdd, sizeof(sSeed));
    cur->next->next = NULL;
}

sSeed advanceSeedsInMap(const sSeed *seed, sSeed *newSeed, const sMap *map, int *hasCut)
{
    for (size_t i = 0; i < map->rangesLength; i++)
    {
        sRange r = map->ranges[i];
        if (seed->begin >= r.sourceStart && seed->begin < r.sourceStart + r.size) // Seed range at least partially in this map range
        {
            unsigned int offset = seed->begin - r.sourceStart;
            if (offset + seed->size < r.size)
            {
                // Seed entirely contained in map range
                printf("Entirely contained !\n");
                *hasCut = 0;
                sSeed toRet = {r.destStart + offset, seed->size};
                return toRet;
            }
            else
            {
                printf("Cutting !\n");
                // Cutting this seed range, and creating a new seed range with the leftovers
                unsigned long seedLeftovers = offset + seed->size - r.size;
                unsigned long seedLeftoversBegin = r.sourceStart + r.size;
                unsigned long newSeedLength = r.size - offset;
                newSeed->begin = seedLeftoversBegin;
                newSeed->size = seedLeftovers;
                *hasCut = 1;
                sSeed toRet = {r.destStart + offset, newSeedLength};
                return toRet;
            }
        }
    }
    // The beginning of the seed is not in a range. Need to find where to cut the seed.
    printf("Not in range... !\n");

    for (size_t i = 0; i < seed->size; i++)
    {
        for (size_t j = 0; j < map->rangesLength; j++)
        {
            sRange r = map->ranges[j];
            if (seed->begin + i >= r.sourceStart && seed->begin + i < r.sourceStart + r.size)
            {
                printf("Partially !\n");

                // The seed range is partially in a map range. We cut it.
                newSeed->begin = seed->begin + i;
                newSeed->size = seed->size - i; // This seed is at least beginning in a range.
                *hasCut = 1;
                sSeed toRet = {seed->begin, i}; // This seed is totally out of map range.
                return toRet;
            }
        }
    }
    // The entire seed is out of map range.
    newSeed = NULL;
    printf("Fully !\n");
    sSeed toRet = {seed->begin, seed->size};
    *hasCut = 0;
    return toRet;
}

int puzzle2(const unsigned long *seeds, const size_t seedsLength, const sMaps *maps)
{
    sSeed tmpSeed = {seeds[0], seeds[1]};
    sLinkedSeed *begin = createLinkedSeed(&tmpSeed);
    for (size_t i = 2; i < seedsLength; i += 2)
    {
        tmpSeed.begin = seeds[i];
        tmpSeed.size = seeds[i + 1];
        addSeed(begin, &tmpSeed);
        printf("seed: %u %u\n", tmpSeed.begin, tmpSeed.size);
    }
    sLinkedSeed *cur = begin;
    sSeed newSeedToAdd;
    int hasCut = 0;
    while (cur->next != NULL)
    {
        sSeed updatedSeed = advanceSeedsInMap(&cur->seed, &newSeedToAdd, &maps->seedToSoil, &hasCut);
        printf("From (%u, %u) -> (%u, %u).", cur->seed.begin, cur->seed.size, updatedSeed.begin, updatedSeed.size);
        cur->seed = updatedSeed;
        if (hasCut)
        {
            addSeed(cur, &newSeedToAdd);
            printf("New seed ! (%u, %u)", newSeedToAdd.begin, newSeedToAdd.size);
        }
        printf("\n");
        cur = cur->next;
    }
    cur = begin;
    while (cur->next != NULL)
    {
        sSeed updatedSeed = advanceSeedsInMap(&cur->seed, &newSeedToAdd, &maps->soilToFertilizer, &hasCut);
        cur->seed = updatedSeed;
        if (hasCut)
        {
            addSeed(cur, &newSeedToAdd);
        }
        cur = cur->next;
    }
    cur = begin;
    while (cur->next != NULL)
    {
        sSeed updatedSeed = advanceSeedsInMap(&cur->seed, &newSeedToAdd, &maps->fertilizerToWater, &hasCut);
        cur->seed = updatedSeed;
        if (hasCut)
        {
            addSeed(cur, &newSeedToAdd);
        }
        cur = cur->next;
    }
    cur = begin;
    while (cur->next != NULL)
    {
        sSeed updatedSeed = advanceSeedsInMap(&cur->seed, &newSeedToAdd, &maps->waterToLight, &hasCut);
        cur->seed = updatedSeed;
        if (hasCut)
        {
            addSeed(cur, &newSeedToAdd);
        }
        cur = cur->next;
    }
    cur = begin;
    while (cur->next != NULL)
    {
        sSeed updatedSeed = advanceSeedsInMap(&cur->seed, &newSeedToAdd, &maps->lightToTemperature, &hasCut);
        cur->seed = updatedSeed;
        if (hasCut)
        {
            addSeed(cur, &newSeedToAdd);
        }
        cur = cur->next;
    }
    cur = begin;
    while (cur->next != NULL)
    {
        sSeed updatedSeed = advanceSeedsInMap(&cur->seed, &newSeedToAdd, &maps->temperatureToHumidity, &hasCut);
        cur->seed = updatedSeed;
        if (hasCut)
        {
            addSeed(cur, &newSeedToAdd);
        }
        cur = cur->next;
    }
    cur = begin;
    while (cur->next != NULL)
    {
        sSeed updatedSeed = advanceSeedsInMap(&cur->seed, &newSeedToAdd, &maps->humidityToLocation, &hasCut);
        cur->seed = updatedSeed;
        if (hasCut)
        {
            addSeed(cur, &newSeedToAdd);
        }
        cur = cur->next;
    }
    cur = begin;
    // Find the lowest beginning
    unsigned long min = ULONG_MAX;
    while (cur->next != NULL)
    {
        min = cur->seed.begin < min ? cur->seed.begin : min;
        cur = cur->next;
    }
    cur = begin;
    // TODO: free the linked list.
    return min;
}

int main()
{
    char *filename = "./input.txt";
    unsigned long *seeds;
    size_t seedsNumber;
    sMaps maps;
    parseInput(filename, &seeds, &seedsNumber, &maps);
    for (size_t i = 0; i < seedsNumber; i++)
    {
        printf("%u\n", seeds[i]);
    }
    printf("size: %u\n", maps.seedToSoil.rangesLength);
    for (size_t i = 0; i < maps.seedToSoil.rangesLength; i++)
    {
        sRange r = maps.seedToSoil.ranges[i];
        printf("%u <- %u (*%u)\n", r.destStart, r.sourceStart, r.size);
    }
    const unsigned int puzzle1Answer = puzzle1(seeds, seedsNumber, &maps);
    printf("Puzzle 1 solution: %u\n", puzzle1Answer);

    const unsigned int puzzle2Answer = puzzle2(seeds, seedsNumber, &maps);
    printf("Puzzle 2 solution: %u\n", puzzle2Answer);

    if (seeds != NULL)
    {
        free(seeds);
        seeds = NULL;
    }
    return 0;
}