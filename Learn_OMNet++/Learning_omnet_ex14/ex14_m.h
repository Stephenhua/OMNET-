//
// Generated file, do not edit! Created by nedtool 5.2 from ex14.msg.
//

#if defined(__clang__)
#  pragma clang diagnostic ignored "-Wreserved-id-macro"
#endif
#ifndef __EX14_M_H
#define __EX14_M_H

#include <omnetpp.h>

// nedtool version check
#define MSGC_VERSION 0x0502
#if (MSGC_VERSION!=OMNETPP_VERSION)
#    error Version mismatch! Probably this file was generated by an earlier version of nedtool: 'make clean' should help.
#endif



/**
 * Class generated from <tt>ex14.msg:17</tt> by nedtool.
 * <pre>
 * message TicTocMsg_ex14
 * {
 *     int source;
 *     int destination;
 *     int hopCount = 0;
 * }
 * </pre>
 */
class TicTocMsg_ex14 : public ::omnetpp::cMessage
{
  protected:
    int source;
    int destination;
    int hopCount;

  private:
    void copy(const TicTocMsg_ex14& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const TicTocMsg_ex14&);

  public:
    TicTocMsg_ex14(const char *name=nullptr, short kind=0);
    TicTocMsg_ex14(const TicTocMsg_ex14& other);
    virtual ~TicTocMsg_ex14();
    TicTocMsg_ex14& operator=(const TicTocMsg_ex14& other);
    virtual TicTocMsg_ex14 *dup() const override {return new TicTocMsg_ex14(*this);}
    virtual void parsimPack(omnetpp::cCommBuffer *b) const override;
    virtual void parsimUnpack(omnetpp::cCommBuffer *b) override;

    // field getter/setter methods
    virtual int getSource() const;
    virtual void setSource(int source);
    virtual int getDestination() const;
    virtual void setDestination(int destination);
    virtual int getHopCount() const;
    virtual void setHopCount(int hopCount);
};

inline void doParsimPacking(omnetpp::cCommBuffer *b, const TicTocMsg_ex14& obj) {obj.parsimPack(b);}
inline void doParsimUnpacking(omnetpp::cCommBuffer *b, TicTocMsg_ex14& obj) {obj.parsimUnpack(b);}


#endif // ifndef __EX14_M_H

