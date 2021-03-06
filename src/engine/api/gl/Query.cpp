//
// Created by Vali on 3/16/2021.
//

#include "Query.hpp"

namespace vd::gl {

    Query::Query(const Type& type)
        : m_Id(0)
        , m_InUse(false)
        , m_Type(type)
    {
    }

    void Query::OnCreate() {
        glGenQueries(1, &m_Id);
    }

    void Query::OnCleanUp() {
        glDeleteQueries(1, &m_Id);
    }

    void Query::Prepare() {
        PassIfCreated();

        glEnable(GL_DEPTH_TEST);
        glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
        glDepthMask(GL_FALSE);
    }

    void Query::Finish() {
        PassIfCreated();

        glDisable(GL_DEPTH_TEST);
        glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
        glDepthMask(GL_TRUE);
    }

    void Query::Start() {
        PassIfCreated();

        glBeginQuery(m_Type, m_Id);
        m_InUse = true;
    }

    void Query::End() {
        PassIfCreated();

        glEndQuery(m_Type);
    }

    bool Query::ResultReady() {
        PassIfCreated();

        if (!m_InUse) {
            return false;
        }

        int result = 0;
        glGetQueryObjectiv(m_Id, GL_QUERY_RESULT_AVAILABLE, &result);

        return result == GL_TRUE;
    }

    bool Query::InUse() {
        return m_InUse;
    }

    int Query::GetResult() {
        PassIfCreated();

        int result = 0;
        glGetQueryObjectiv(m_Id, GL_QUERY_RESULT, &result);
        m_InUse = false;

        return result;
    }

}