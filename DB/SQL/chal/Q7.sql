WITH MACount AS (
    SELECT 
        bbooks.member_id, 
        b.author_id
    FROM BorrowedBooks bbooks
    JOIN Books b ON bbooks.book_id = b.book_id
    WHERE bbooks.borrow_date BETWEEN '2022-03-10' AND '2022-09-10'
    GROUP BY bbooks.member_id, b.author_id
),

FilterMembers AS (
    SELECT 
        MAC.member_id
    FROM MACount MAC
    GROUP BY MAC.member_id
    HAVING COUNT(DISTINCT MAC.author_id) >= 3
),

MemberBorrow AS (
    SELECT 
        FM.member_id,
        COUNT(*) AS total_borrows
    FROM FilterMembers FM
    JOIN BorrowedBooks bbooks ON FM.member_id = bbooks.member_id
    WHERE bbooks.borrow_date BETWEEN '2022-03-10' AND '2022-09-10'
    GROUP BY FM.member_id
    ORDER BY total_borrows DESC
    LIMIT 2
)

SELECT 
    m.name AS member_name,
    MB.total_borrows AS total_books_borrowed,
    a.name AS author_name,
    GROUP_CONCAT(b.title ORDER BY b.title) AS borrowed_books
FROM MemberBorrow MB
JOIN Members m ON MB.member_id = m.member_id
JOIN BorrowedBooks bbooks ON MB.member_id = bbooks.member_id
JOIN Books b ON bbooks.book_id = b.book_id
JOIN Authors a ON b.author_id = a.author_id
WHERE bbooks.borrow_date BETWEEN '2022-03-10' AND '2022-09-10'
GROUP BY m.name, MB.total_borrows, a.name
ORDER BY MB.total_borrows DESC, m.name, a.name;
