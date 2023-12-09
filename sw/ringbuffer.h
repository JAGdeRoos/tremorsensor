/******************************************************************************
 * Project    : Tremor Sensor Inator
 * Author     : Ewout Boks
 * Description: Ringbuffer class from DSP class
******************************************************************************/

#ifndef _RINGBUFFER_H_
#define _RINGBUFFER_H_

#include <Arduino.h>
template <typename ttype>
class ringbuffer
{
  public:
    /*! @brief The standard constructor for a circular buffer with dynamic memory allocation.
     * @param a : The number of elements in the buffer */
    explicit ringbuffer(const unsigned short a) : elements(new ttype[a]),
      size_(a),
      dynAlloc(true),
      readPtr(elements),
      writePtr(elements)
    {
      head_pos = 0;
    };

    /*! @brief The standard constructor for a circular buffer with static external memory allocation.
     * @param a : The number of elements in the buffer
     * @param bufferInit : a pointer at an external array with a elements. */
    ringbuffer(const unsigned short a,
                   ttype const * const bufferInit) : elements(const_cast<ttype *>(bufferInit)),
      size_(a),
      dynAlloc(false),
      readPtr(elements),
      writePtr(elements)
    {
      head_pos = 0;
    };

    /*! @brief the destructor */
    ~ringbuffer()
    {
      if (true == dynAlloc)
        delete[] elements;
    };

    /*! @brief The reset function should take care of resetting the buffer to the initial state. */
    void reset()
    {
      for (int x = 0; x < size_; x++) {
        elements[x] = float(NULL);
      }
      head_pos = 0;
    };

    /*! @brief This function gives the number of elements in the buffer. */
    unsigned short giveSize() const
    {
      return size_;
    };

    /*! @brief Write a number to the ring buffer.
     * @note both read and write pointer must be updated. */
    void write_(const ttype getal)
    {
      head_pos++;
      if (head_pos > size_ - 1) {
        head_pos = 0;
      }
      elements[head_pos] = getal;
    };

    /*! @brief Read the latest  sample x[n].
     * @note move the readPtr. */
    ttype read_()
    {
      return elements[head_pos];
    };

    /*! @brief Read the sample x[n-index].
     * @note do not move the readPtr. */
    ttype read_(const unsigned int index) const
    {
      int read_pos = head_pos;
      int ans = read_pos - index;
      if (ans < 0) {
        for (unsigned int x = 0; x < index; x++) {
          read_pos--;
          if (read_pos < 0) {
            read_pos = size_ - 1;
          }
        }
      }
      else {
        read_pos -= index;

      }

      return elements[read_pos];
    };

    /*! @brief Read the buffer value at [index] in an absolute way,
     * ie not with respect to the write pointer.
     * @note monitor the buffer limits in the function. */
    ttype operator [](const unsigned int index) const
    {
      int read_pos = head_pos;
      if (read_pos - index < 0) {
        for (unsigned int x = 0; x < index; x++) {
          read_pos--;
          if (read_pos < 0) {
            read_pos = size_ - 1;
          }
        }
      }
      return elements[read_pos];
    }

    /*! @brief calculate the sum of all values present in the buffer.
     * @return the sum value. */
    ttype sum() const
    {
      ttype sumvalue = 0;
      for (auto &el : elements) {
        sumvalue += el;
      }
      return sumvalue;
    };

    /*! @brief calculate the average of all values present in the buffer.
     * @return the average value. */
    ttype average() const
    {
      ttype avevalue = 0;
      for (int i = 0; i < size_; i++) {
        avevalue += elements[i];
      }
      avevalue = avevalue / size_;
      return avevalue;
    };

  protected:

  private:
    ttype * const         elements;       /* Deze ptr wijst naar de buffer. */
    const unsigned short  size_;          /* het aantal elementen in de buffer. */
    const bool            dynAlloc;
    ttype                 *readPtr,       /* ptr implementatie van de ringbuffer.*/
                          *writePtr;      
    int                   head_pos;
};

#endif
