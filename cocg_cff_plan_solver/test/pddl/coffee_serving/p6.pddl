(define (problem CM_R1_C6)
    (:domain coffee_serving)

    (:objects
        r1 - robot
        ps p1 p2 pg - location
        c1 c2 c3 c4 c5 c6 - coffee
    )

    (:init
        (robot_at r1 ps)
        (hand_empty r1)
        (unknown
            (coffee_at c1 p1))
        (unknown
            (coffee_at c1 p2))

        (oneof
            (coffee_at c1 p1)
            (coffee_at c1 p2)
        )
        (unknown
            (coffee_at c2 p1))
        (unknown
            (coffee_at c2 p2))

        (oneof
            (coffee_at c2 p1)
            (coffee_at c2 p2)
        )
        (unknown
            (coffee_at c3 p1))
        (unknown
            (coffee_at c3 p2))

        (oneof
            (coffee_at c3 p1)
            (coffee_at c3 p2)
        )
        (unknown
            (coffee_at c4 p1))
        (unknown
            (coffee_at c4 p2))

        (oneof
            (coffee_at c4 p1)
            (coffee_at c4 p2)
        )
        (unknown
            (coffee_at c5 p1))
        (unknown
            (coffee_at c5 p2))

        (oneof
            (coffee_at c5 p1)
            (coffee_at c5 p2)
        )
        (unknown
            (coffee_at c6 p1))
        (unknown
            (coffee_at c6 p2))

        (oneof
            (coffee_at c6 p1)
            (coffee_at c6 p2)
        )

    )
    (:goal
        (and
            (coffee_at c1 pg)
            (coffee_at c2 pg)
            (coffee_at c3 pg)
            (coffee_at c4 pg)
            (coffee_at c5 pg)
            (coffee_at c6 pg)
        )
    )
)