/**
 * \file csvline.h
 * \brief Manage a line of a csv file.
 *
 * \author Marco Boeris Frusca
 *
 */
#ifndef CSVLINE_H_
#define CSVLINE_H_

#include "Arduino.h"
#include "stringparser.h"

namespace sdp
{
  namespace message
  {
    /**
     * The CSVLine class describes a generic line of a CSV file.
     *
     * \note It is has been fixed a max size for the line size (128 chars).
     *
     */
    class CSVLine
    {
      public:

        //! Max size for a CSV line
        static const size_t LINE_SIZE;

        //! Field Separator
        static const char FS;

        /**
         * Default Constructor.
         *
         */
        CSVLine();

        /**
         * Copy-Constructor
         *
         */
        CSVLine(const CSVLine& l);

        /**
         * Destructor.
         *
         */
        virtual ~CSVLine();

        /**
         * Initializes object from a CSV line.
         *
         * \param[in] line buffer where CSV line is stored
         * \param[in] length size of the buffer
         *
         */
        void set(const char* line, size_t length);

        /**
         * Gets a fields of the CSV line as a string.
         *
         * \param[in] index field position
         *
         * \return pointer to the field (char array) or 0 if invalid index
         */
        char* getItem(size_t index) { return (index < m_nf)?&m_line[m_fields[index]]:0; }

        /**
         * Gets a fields of the CSV line as an integer.
         *
         * \param[in] index field position
         * \param[out] n field
         *
         * \return false if invalid index or wrong format (not integer), true otherwise
         */
        bool getItemAsInt(size_t index, int &n);

        /**
         * Gets a fields of the CSV line as an long integer.
         *
         * \param[in] index field position
         * \param[out] n field
         *
         * \return false if invalid index or wrong format (not long integer), true otherwise
         */
        bool getItemAsLong(size_t index, long &n);

        /**
         * Gets a fields of the CSV line as an float.
         *
         * \param[in] index field position
         * \param[out] n field
         *
         * \return false if invalid index or wrong format (not float), true otherwise
         */
        bool getItemAsFloat(size_t index, long &n);

        /**
         * Gets the fields number of CSV line.
         *
         * \return number of fields
         */
        size_t NF() { return m_nf; };

      private:
        //! Internal buffer for storage the CSV line
        char *m_line;

        //! line length
        size_t m_length;

        //! Pointer to the array, its elements point to the first character og each field
        size_t *m_fields;

        //! Number of fields
        size_t m_nf;

        //! Flag  to check if it is a copy
        bool m_isCopy;
    };

  } /* namespace message */
} /* namespace sdp */

#endif /* CSVLINE_H_ */
