// Sequence.h

#ifndef SEQUENCE_INCLUDED
#define SEQUENCE_INCLUDED


template<typename ItemType> class Sequence
{
  public:
    Sequence() {
        createEmpty();
    }
    bool empty() const;
    int size() const;

    int insert(int pos, const ItemType& value) {
        if (pos < 0  ||  pos > m_size)
            return -1;

        Node* p = nodeAtPos(pos);
        insertBefore(p, value);
        
        return pos;
    }
      
    int insert(const ItemType& value) {
        Node* p;
        int pos;
        for (p = m_head->m_next, pos = 0; p != m_head  && value > p->m_value; p = p->m_next, pos++)
            ;
        
        insertBefore(p, value);
        return pos;
    }

    bool erase(int pos) {
        if (pos < 0  ||  pos >= m_size)
            return false;

        Node* p = nodeAtPos(pos);
        doErase(p);

        return true;
    }

    int remove(const ItemType& value) {
        int count = 0;
        
        Node* p = m_head->m_next;
        while (p != m_head)
        {
            if (p->m_value == value)
            {
                    count++;
                    p = doErase(p); // p now points to successor of erased Node
            }
            else
                    p = p->m_next;  // p now points to successor of non-erased Node
        }
        return count;
    }

    bool get(int pos, ItemType& value) const {
        if (pos < 0  ||  pos >= m_size)
            return false;

        Node* p = nodeAtPos(pos);
        value = p->m_value;

        return true;
    }

    bool set(int pos, const ItemType& value) {
        if (pos < 0  ||  pos >= m_size)
            return false;

        Node* p = nodeAtPos(pos);
        p->m_value = value;

        return true;
    }

    int find(const ItemType& value) const {
        int pos = 0;
        Node* p = m_head->m_next;
        for ( ; p != m_head  &&  p->m_value != value; p = p->m_next, pos++)
            ;
        return p == m_head ? -1 : pos;
    }

    void swap(Sequence& other) {
        Node* p = other.m_head;
        other.m_head = m_head;
        m_head = p;

        int s = other.m_size;
        other.m_size = m_size;
        m_size = s;
    }

      // Housekeeping functions
    ~Sequence() {
        while (m_head->m_next != m_head)
            doErase(m_head->m_next);
        
        delete m_head;
    }
    
    Sequence(const Sequence<ItemType>& other) { // here
        
        createEmpty();

        for (Node* p = other.m_head->m_next; p != other.m_head; p = p->m_next)
            insertBefore(m_head, p->m_value);
    }
    
    Sequence<ItemType>& operator=(const Sequence<ItemType>& rhs) { // here because it returns the value, not the pointer
        if (this != &rhs)
        {
            Sequence temp(rhs);
            swap(temp);
        }
        return *this;
    }

  private:

    struct Node
    {
        ItemType m_value;
        Node*    m_next;
        Node*    m_prev;
    };

    Node* m_head;
    int   m_size;

    void createEmpty() {
        m_size = 0;

        m_head = new Node;
        m_head->m_next = m_head;
        m_head->m_prev = m_head;
    }
    
    void insertBefore(Node* p, const ItemType& value){
      Node* newp = new Node;
      newp->m_value = value;
        
      newp->m_prev = p->m_prev;
      newp->m_next = p;
      newp->m_prev->m_next = newp;
      newp->m_next->m_prev = newp;

      m_size++;
    }
    
    typename Sequence<ItemType>::Node* doErase(Node* p) { // here because it returns the value, not the pointer
      Node* pnext = p->m_next;

      p->m_prev->m_next = p->m_next;
      p->m_next->m_prev = p->m_prev;
      delete p;

      m_size--;

      return pnext;
    }

    typename Sequence<ItemType>::Node* nodeAtPos(int pos) const { // here because it returns the value, not the pointer
        Node* p;

        if (pos <= m_size / 2)
        {
            p = m_head->m_next;
            for (int k = 0; k != pos; k++)
                p = p->m_next;
        }
        else
        {
            p = m_head;
            for (int k = m_size; k != pos; k--)
                p = p->m_prev;
        }

        return p;
    }
    
};

// Declarations of non-member functions

template<typename ItemType> // template

int subsequence(const Sequence<ItemType>& seq1, const Sequence<ItemType>& seq2) { // here
    if (seq2.empty())
        return -1;

      // Walk through seq1

    for (int pos = 0; pos <= seq1.size() - seq2.size(); pos++)
    {
          // Assume there's a match starting at pos

        bool allMatched = true;

          // Check if all corresponding positions match

        for (int k = 0; k < seq2.size(); k++)
        {
            ItemType v1;
            ItemType v2;
            seq1.get(pos+k, v1);
            seq2.get(k, v2);
            if (v1 != v2)
            {
                allMatched = false;
                break;
            }
        }

          // If we never found a mismatch, we've found the match.

        if (allMatched)
            return pos;
    }

      // If we never found a match, there is none.

    return -1;
}

template<typename ItemType> // template

void interleave(const Sequence<ItemType>& seq1, const Sequence<ItemType>& seq2, Sequence<ItemType>& result) { // here

  Sequence<ItemType> res; // here

  int n1 = seq1.size();
  int n2 = seq2.size();
  int nmin = (n1 < n2 ? n1 : n2);
  int resultPos = 0;
  for (int k = 0; k < nmin; k++)
  {
      ItemType v;
      seq1.get(k, v);
      res.insert(resultPos, v);
      resultPos++;
      seq2.get(k, v);
      res.insert(resultPos, v);
      resultPos++;
  }


  const Sequence<ItemType>& s = (n1 > nmin ? seq1 : seq2); // here!
  int n = (n1 > nmin ? n1 : n2);
  for (int k = nmin ; k < n; k++)
  {
      ItemType v;
      s.get(k, v);
      res.insert(resultPos, v);
      resultPos++;
  }

  result.swap(res);
}


// Inline implementations

//inline
template<typename ItemType>
int Sequence<ItemType>::size() const
{
    return m_size;
}

//inline
template<typename ItemType>
bool Sequence<ItemType>::empty() const
{
    return size() == 0;
}




#endif // SEQUENCE_INCLUDED
