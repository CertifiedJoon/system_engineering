SELECT 
    B.publisher_id,
    B.author_id,
    ROUND(
        IFNULL(
            (SELECT COUNT(*) FROM BorrowedBooks BB1 WHERE BB1.book_id = B.book_id) * 100.0 / 
            (SELECT COUNT(*) FROM BorrowedBooks BB2 JOIN Books B2 ON BB2.book_id = B2.book_id WHERE B2.publisher_id = B.publisher_id), 
        0), 
        1
    ) AS portion_of_published_books
FROM Books B
GROUP BY B.publisher_id, B.author_id
ORDER BY B.publisher_id ASC, B.author_id ASC;