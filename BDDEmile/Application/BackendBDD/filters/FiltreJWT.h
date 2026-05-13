//
// Created by Emile Guillerm on 07.05.2026.
//

#ifndef BACKENDBDD_FILTREJWT_H
#define BACKENDBDD_FILTREJWT_H
#include <drogon/HttpController.h>
#include <drogon/utils/coroutine.h>

class FiltreJWT : public drogon::HttpCoroFilter<FiltreJWT>{
public:
    drogon::Task<drogon::HttpResponsePtr> doFilter(drogon::HttpRequestPtr req) override;
};


#endif //BACKENDBDD_FILTREJWT_H