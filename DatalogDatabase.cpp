#ifndef CS236_PROJECT3_T2K_DATALOG_DATABASE_CPP
#define CS236_PROJECT3_T2K_DATALOG_DATABASE_CPP

#include "DatalogDatabase.h"

/**
 * @brief Update the internal Database with the data from the Scheme list of the DatalogProgram.
 */
void DatalogDatabase::evaluateSchemes()
{
    // TODO Test
    for (Predicate predicate : *(dlProgram->getSchemes()))
    {
        std::vector<std::string> attributeNames = predicate.getParamStrings();
        std::string relationName = predicate.getId();
        Scheme scheme(attributeNames);
        Relation relation(relationName, scheme);
        addRelation(relation);
    }
}

/**
 * @brief Update the internal Database with the data from the Fact list of the DatalogProgram.
 */
void DatalogDatabase::evaluateFacts()
{
    // TODO
    // TODO? Check if facts ever contain variables (I'm assuming they don't)
    for (Predicate predicate : *(dlProgram->getFacts()))
    {
        std::vector<std::string> values = predicate.getParamStrings();
        std::string relationName = predicate.getId();
        Tuple tuple(values);
        addTuple(tuple, relationName);
    }
}

/**
 * @brief Update the internal Database with the data from the Query list of the DatalogProgram.
 */
void DatalogDatabase::evaluateQueries()
{
    // TODO Return? Print?
    // Evaluating Queries

    // For each query in the Datalog program:
    for (Predicate predicate : *(dlProgram->getQueries()))
    {
        // Get the Relation from the Database with the same name as the predicate name in the query.

        std::vector<Parameter> *params = predicate.getParams();
        std::string relationName = predicate.getId();

        int index = getIndex(relationName);
        if (index < 0)
        {
            std::cerr << "[ERROR] Invalid query: no such relation." << std::endl;
            throw;
        }
        Relation relation = relations.at(static_cast<unsigned int>(index));

        // Use a sequence of select, project, and rename operations on the Database to evaluate the query.
        relation = selectForQuery(relation, params);
        relation = projectForQuery(relation, params);
        relation = renameForQuery(relation, params);
    }
}

/**
 * @brief Apply SELECT operations to select the tuples from the given Relation that match the query.
 *
 * @param relation The relation to apply SELECT to.
 * @param params The Parameter list of the current query.
 * @return const Relation
 */
const Relation DatalogDatabase::selectForQuery(Relation relation, const std::vector<Parameter> *params) const
{
    // Use one or more select operations to select the tuples from the Relation that match the query. Iterate over the parameters of the query: If the parameter is a constant, select the tuples from the Relation that have the same value as the constant in the same position as the constant. If the parameter is a variable and the same variable name appears later in the query, select the tuples from the Relation that have the same value in both positions where the variable name appears.
    //TODO Error if params and relation.scheme don't match in size?
    for (unsigned int ui = 0; ui < params->size(); ui++)
    {
        Parameter param = params->at(ui);
        if (param.isVariable())
        {
            // Check if it's elsewhere
            for (unsigned int uj = ui; uj < params->size(); uj++)
            {
                if (params->at(uj).isVariable() && params->at(uj).getValue() == param.getValue())
                {
                    relation = relation.selectForEqualVariables(ui, uj);
                }
            }
        }
        else
        {
            relation = relation.selectForConstant(ui, param.getValue());
        }
    }

    return relation;
}

/**
 * @brief Apply PROJECT operations to keep only attributes that correspond to the positions of the variables in the query.
 *
 * @param relation The relation to apply PROJECT to.
 * @param params The Parameter list of the current query.
 * @return const Relation
 */
const Relation DatalogDatabase::projectForQuery(Relation relation, const std::vector<Parameter> *params) const
{
    // After selecting the matching tuples, use the project operation to keep only the columns from the Relation that correspond to the positions of the variables in the query. Make sure that each variable name appears only once in the resulting relation. If the same name appears more than once, keep the first column where the name appears and remove any later columns where the same name appears. (This makes a difference when there are other columns in between the ones with the same name.)

    std::vector<unsigned int> u_attributeIndices;

    for (unsigned int ui = 0; ui < params->size(); ui++)
    {
        Parameter param = params->at(ui);
        if (param.isVariable())
        {
            // Make sure this attribute wasn't already added at a different index
            bool contains = false;
            for (unsigned int uj = 0; uj < ui; uj++)
            {
                if (params->at(uj).getValue() == param.getValue())
                {
                    contains = true;
                }
            }
            if (!contains)
            {
                u_attributeIndices.push_back(ui);
            }
        }
    }
    return relation.project(u_attributeIndices);
}

/**
 * @brief Apply RENAME operations to rename the scheme of the Relation to the names of the variables found in the query.
 *
 * @param relation The relation to apply RENAME to.
 * @param params The Parameter list of the current query.
 * @return const Relation
 */
const Relation DatalogDatabase::renameForQuery(const Relation relation, const std::vector<Parameter> *params) const
{
    Scheme scheme = relation.getScheme();

    unsigned int u_originalIndex = 0;  // Index of current parameter
    unsigned int u_projectedIndex = 0; // Index of current parameter after projection
    while (u_originalIndex < params->size() && u_projectedIndex < scheme.size())
    {
        if (params->at(u_originalIndex).isVariable())
        {
            std::string oldName = scheme.at(u_projectedIndex);
            std::string newName = params->at(u_originalIndex).getValue();
            relation.rename(oldName, newName);
            u_projectedIndex++;
        }
        u_originalIndex++;
    }
    return relation;
}

std::string DatalogDatabase::toString() const
{
    std::stringstream ss;
    ss << "-- Datalog Database --" << std::endl
       << std::endl;
    for (Relation relation : this->relations)
    {
        ss << "Relation '" << relation.getName() << "'" << std::endl;
        ss << relation.toString() << std::endl;
    }
    return ss.str();
}

#endif