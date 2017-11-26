#include "TemplateMap.hh"

int main(int lb, char** par)
{
    TemplateMap<long/*MDMLoss*/, long> lMap;
    long Id, RId, vFid, fid;
    MDMLoss mdm;

    RId = 11;

    printf("%d ", lMap.NewMapForId(Id, RId));
    printf("%ld --> %ld\n", RId, Id);
    printf("%d ", lMap.NewMapForId(Id, RId));
    printf("%ld --> %ld\n", RId, Id);

    RId = 10;
    printf("%d ", lMap.NewMapForId(Id, RId));
    printf("%ld --> %ld\n", RId, Id);


    printf("%d ", lMap.NewMap(RId));
    printf("%ld\n", RId);
    printf("%d ", lMap.NewMap(RId));
    printf("%ld\n", RId);

    RId = 13;

    printf("%d ", lMap.NewMapForId(Id, RId));
    printf("%ld --> %ld\n", RId, Id);

    RId = 9;
    vFid = -1001;
    fid = 1000;

    printf("insert: %ld\n", lMap.InsertKey(RId, fid, vFid));
    return 0;
}

