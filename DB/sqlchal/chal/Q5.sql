WITH RelevantBooks AS (
    SELECT 
        b.book_id
    FROM BorrowedBooks bbooks
    JOIN Books b ON bbooks.book_id = b.book_id
    WHERE bbooks.borrow_date BETWEEN '2022-03-10' AND '2022-09-10'
    GROUP BY b.book_id
    HAVING COUNT(DISTINCT bbooks.member_id) >= 2
)

SELECT 
    b.title AS title,
    a.name AS author_name,
    p.name AS publisher_name,
    m.name AS member_name,
    bbooks.borrow_date AS borrow_date
FROM RelevantBooks rbooks
JOIN Books b ON rbooks.book_id = b.book_id
JOIN Authors a ON b.author_id = a.author_id
JOIN Publishers p ON b.publisher_id = p.publisher_id
JOIN BorrowedBooks bbooks ON b.book_id = bbooks.book_id
JOIN Members m ON bbooks.member_id = m.member_id
WHERE bbooks.borrow_date BETWEEN '2022-03-10' AND '2022-09-10'
ORDER BY b.title, m.name, bbooks.borrow_date;
