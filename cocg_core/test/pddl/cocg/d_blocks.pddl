(define (domain blocksworld)
    (:requirements :strips :typing :disjunctive-preconditions)

    (:types
        block robot location
    )
    (:predicates
        (robot_at ?r - robot ?p - location)
        (hand_empty ?r - robot)
        (block_on_hand ?r - robot ?b - block)
        (block_at ?b - block ?p - location)
    )

    (:action locate_robot_at
        :parameters (?r - robot ?p - location)
        :observe
        (robot_at ?r ?p)
    )

    (:action sense_hand_empty
        :parameters (?r - robot)
        :observe
        (hand_empty ?r)
    )

    (:action sense_block_on_hand
        :parameters (?r - robot ?b - block)
        :observe
        (block_on_hand ?r ?b)
    )

    (:action sense_block_at
        :parameters (?r - robot ?b - block ?p - location)
        :precondition (and
            (robot_at ?r ?p)
        )
        :observe
        (block_at ?b ?p)
    )

    (:action move_to
        :parameters (?r - robot ?pf - location ?pt - location)
        :precondition (and (robot_at ?r ?pf) (hand_empty ?r))
        :effect (and (not (robot_at ?r ?pf)) (robot_at ?r ?pt))
    )

    (:action move_with_block
        :parameters (?r - robot ?b - block ?pf - location ?pt - location)
        :precondition (and (robot_at ?r ?pf) (block_on_hand ?r ?b))
        :effect (and (not (robot_at ?r ?pf)) (robot_at ?r ?pt))
    )

    (:action pickup
        :parameters (?r - robot ?b - block ?p - location)
        :precondition (and (robot_at ?r ?p) (block_at ?b ?p) (hand_empty ?r))
        :effect (and (not (block_at ?b ?p)) (not (hand_empty ?r)) (block_on_hand ?r ?b))
    )

    (:action place
        :parameters (?r - robot ?b - block ?p - location)
        :precondition (and (robot_at ?r ?p) (block_on_hand ?r ?b))
        :effect (and (not (block_on_hand ?r ?b)) (hand_empty ?r) (block_at ?b ?p))
    )

)

;; Notice: predicate or action names should not contain '-' (subtract)
;; or, parser will collide with the numeric