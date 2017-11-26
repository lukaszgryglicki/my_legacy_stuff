#ifndef __TEMPLATE_MAP_HH__
#define __TEMPLATE_MAP_HH__

#include <map>

using namespace std;

template<typename K, typename V> 
class TemplateMap
{
    public:

    TemplateMap() : m_lNewId(0)
    {
    }

    bool NewMap(long&);
    bool NewMapForId(long&, long);

    bool FindKey(long, const K&, const V&) const;
    bool InsertKey(long, const K&, const V&);

    private:

    map< long, map<K, V> > m_map;
    typename map< long, map<K, V> > :: iterator m_it;
    typename map<K, V>::iterator m_itemIt;
    long m_lNewId;
};


template<typename K, typename V> bool TemplateMap<K, V> :: NewMap(long& _id)
{
    return NewMapForId( _id, ++ m_lNewId );
}

template<typename K, typename V> bool TemplateMap<K, V> :: InsertKey(long mKey, const K& iKey, const V& iVal)
{
    m_it = m_map.find( mKey );

    if (FindKey(mKey, iKey, iVal))
    {
	return false;
    }

    if (m_it != m_map.end()) 
    {
	(*m_it)->second->insert(pair<K, V>(iKey, iVal));
    }
    else 
    {
	return false;
    }
}

template<typename K, typename V> bool TemplateMap<K, V> :: FindKey(long mKey, const K& iKey, const V& iVal) const
{
    m_it = m_map.find( mKey );

    if (m_it != m_map.end()) 
    {

	m_itemIt = (*m_it)->second->find( iKey);

	if (m_itemIt == (*m_it)->second->end())
	{
	    return false;
	}
	else
	{
	    iVal = (*m_itemIt)->second;
	}

	return true;
    }
    else 
    {
	return false;
    }

}



template<typename K, typename V> bool TemplateMap<K, V> :: NewMapForId(long& _id, long _reqId)
{
    m_it = m_map.find(_reqId);

    if (m_it == m_map.end()) 
    {
	map<K, V> newMap;

	m_map.insert( pair<long, map<K, V> >(_reqId, newMap) );
	if (_reqId > m_lNewId) m_lNewId = _reqId;
	_id = _reqId;

	return true;
    }
    else 
    {
	_id = -1;
	return false;
    }
}

struct MDMLoss
{
 public:
    MDMLoss(long, long);
    MDMLoss();

 private:
    long mdm_id, etype;

};

MDMLoss :: MDMLoss()
{
    mdm_id = etype = 0;
}

MDMLoss :: MDMLoss(long _mdm_id, long _etype)
{
    mdm_id = _mdm_id;
    etype = _etype;
}

#endif
