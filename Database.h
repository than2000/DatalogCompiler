#ifndef CS236_PROJECT3_T2K_DATABASE_H
#define CS236_PROJECT3_T2K_DATABASE_H

#include <vector>
#include "Relation.h"

class Database
{
protected:
    std::vector<Relation> relations;

    bool hasRelation(std::string name)
    {
        return getIndex(name) != -1;
    }

    int getIndex(std::string relationName)
    {
        for (unsigned int ui = 0; ui < relations.size(); ui++)
        {
            if (relations.at(ui).getName() == relationName)
            {
                return static_cast<int>(ui);
            }
        }
        return -1;
    }

    const Relation getRelation(std::string relationName)
    {
        int index = getIndex(relationName);
        if (!hasRelation(relationName))
        {
            std::cerr << "[ERROR] Tried to get a relation that isn't in the database." << std::endl;
            throw;
        }
    }

    // Update a relation in the database. Does not affect relation names.
    void updateRelation(std::string relationName, Relation relation)
    {
        int index = getIndex(relationName);
        relation.setName(relationName);
        relations.at(index) = relation;
    }

    ////
    ////
    ////
    ////

public:
    void addRelation(Relation relation)
    {
        if (hasRelation(relation.getName()))
        {
            std::cerr << "[ERROR] Tried to add a relation whose name is already in use." << std::endl;
            throw;
        }
        relations.push_back(relation);
    }

    void addTuple(Tuple tuple, std::string relationName)
    {
        int index = getIndex(relationName);
        if (index == -1)
        {
            std::cerr << "[ERROR] Tried to add a tuple to a relation that doesn't exist." << std::endl;
            throw;
        }
        relations.at(static_cast<unsigned int>(index)).addTuple(tuple);
    }
};

#endif
